#include "uart_driver.h"
#include <asm/ioctls.h>
#include <asm/termbits.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <sys/poll.h>
#include <deque>
#include <unistd.h>
#include <iostream>
#include <string.h>
#include <uart_struct.h>

// #define DBG

// rx buffer deep copied between receive thread and uart_driver
divergence_packet_t uart_packet;

SerialPort::SerialPort(const std::string &port, int uart_speed)
	: receiver_thread_(),
	  receiver_thread_should_exit_(false),
	  serial_port_fd_(-1) {

	valid_uart_message_received = false;
	if (!connectSerialPort(port, uart_speed)) {
		std::cout << "[Serial] Error. Can't connect to port" << std::endl;
	}

	if (!startReceiverThread()) {
		std::cout << "[Serial] Error. Can't start receiver thread" << std::endl;
	}
	
	// if (!startTransmitThread()) {
	// 	std::cout << "[Serial] Error. Can't start transmit thread" << std::endl;
	// }
}

SerialPort::~SerialPort() { disconnectSerialPort(); }

bool SerialPort::connectSerialPort(const std::string &port, int uart_speed) {
	// Open serial port
	// O_RDWR - Read and write
	// O_NOCTTY - Ignore special chars like CTRL-C
	serial_port_fd_ = open(port.c_str(), O_RDWR | O_NOCTTY);

	if (serial_port_fd_ == -1) {
		std::cout << "[Serial] Error. Can't open serial port" << std::endl;
		return false;
	}
	if (!configSerialPort(uart_speed)) {
		close(serial_port_fd_);
		std::cout << "[Serial] Error. Can't configure serial port" << std::endl;
		return false;
	}
	return true;
}

bool SerialPort::startReceiverThread() {
	// Start watchdog thread
	try {
		receiver_thread_ = std::thread(&SerialPort::serialPortReceiveThread, this);
	} catch (...) {
		return false;
	}
	return true;
}

bool SerialPort::startTransmitThread() {
	// Start watchdog thread
	try {
		transmit_thread_ = std::thread(&SerialPort::serialPortTransmitThread, this);
	} catch (...) {
		return false;
	}
	return true;
}

bool SerialPort::configSerialPort(int uart_speed) const {
	// clear config
	fcntl(serial_port_fd_, F_SETFL, 0);
	// read non blocking, return 0 if nothing is read
	fcntl(serial_port_fd_, F_SETFL, FNDELAY);

	struct termios2 uart_config;
	/* Fill the struct for the new configuration */
	ioctl(serial_port_fd_, TCGETS2, &uart_config);

	// https://blog.mbedded.ninja/programming/operating-systems/linux/linux-serial-ports-using-c-cpp/

	// Output flags - Turn off output processing
	// no CR to NL translation, no NL to CR-NL translation,
	// no NL to CR translation, no column 0 CR suppression,
	// no Ctrl-D suppression, no fill characters, no case mapping,
	// no local output processing
	uart_config.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OPOST);

	// Input flags - Turn off input processing
	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// no input parity check, don't strip high bit off,
	// no XON/XOFF software flow control
	uart_config.c_iflag &=
		~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

	// No line processing:
	// echo off
	// echo newline off
	// canonical mode off,
	// extended input processing off
	// signal chars off
	uart_config.c_lflag &= ~(ECHO | ECHOE | ECHONL | ICANON | IEXTEN | ISIG);

	// Turn off character processing
	// Turn off odd parity
	uart_config.c_cflag &= ~(CSIZE | PARODD | CBAUD);

	// Disable parity generation on output and parity checking for input.
	uart_config.c_cflag &= ~PARENB;

	// use one stop bit
	uart_config.c_cflag &= ~CSTOPB;

	// No output processing, force 8 bit input
	uart_config.c_cflag |= CS8;

	// Enable a non standard baud rate
  	uart_config.c_cflag |= BOTHER;

	// B115200, may require termios header for set default speed
	const speed_t spd = uart_speed;
	uart_config.c_ispeed = spd;
	uart_config.c_ospeed = spd;

	// error in making settings
	if (ioctl(serial_port_fd_, TCSETS2, &uart_config) < 0) {
		return false;
	}
	std::cout << "[Serial] Configured" << std::endl;
	return true;
}

void SerialPort::serialPortReceiveThread() {

	struct pollfd fds[1];
	fds[0].fd = serial_port_fd_;
	fds[0].events = POLLIN;
	std::cout << "[Serial] ReceiveThread spawned" << std::endl;

	// init of thread
	uint8_t init_buf[10];
	while (read(serial_port_fd_, init_buf, sizeof(init_buf)) > 0) {
		// On startup, as long as we receive something, we keep reading to ensure
		// that the first byte of the first poll is the start of an SBUS message
		// and not some arbitrary byte.
		// This should help to get the framing in sync in the beginning.
		usleep(100);
	}

	// A lot of heap?!
	std::deque<uint8_t> bytes_reverse_buf;

	// while atomic lock of thread
	while (!receiver_thread_should_exit_) {

		// Buffer to read bytes from serial port. We make it large enough to
		// potentially contain 4 sbus messages but its actual size probably does
		// not matter too much
		// TODO: 4 struct size
		uint8_t read_buf[4 * uartFrameLength_];

		if (poll(fds, 1, kPollTimeoutMilliSeconds_) > 0) {
			if (fds[0].revents & POLLIN) {

				// REMARK: Take into account that read() reads (obviously) but also frees what it reads
				const ssize_t nread = read(serial_port_fd_, read_buf, sizeof(read_buf));
				static long int cnt = 0;
				cnt ++;

				for (ssize_t i = 0; i < nread; i++) {
					bytes_reverse_buf.push_front(read_buf[i]);
					// printf("0x%02x,", read_buf[i]);
				}

				valid_uart_message_received = false;
				memset(uart_msg_bytes, 0, uartFrameLength_ + 3);

				// The potential message should at least the size of the expected array
				while (bytes_reverse_buf.size() >= uartFrameLength_ + 3) {

					if (bytes_reverse_buf.front() == FooterByte_ && bytes_reverse_buf[uartFrameLength_ + 2] == HeaderByte_) {

						// Populate the uart-rx struct
						for (uint8_t i = 0; i < uartFrameLength_+3; i++) {
							uart_msg_bytes[uartFrameLength_ + 3 - 1 - i] = bytes_reverse_buf.front();
							bytes_reverse_buf.pop_front();
						}
						valid_uart_message_received = true;

						memcpy(&uart_packet, &(this->uart_msg_bytes[1]), uartFrameLength_);

            			// perfect deparsing from driver.. :)
						std::cout << "reading cnt: " << cnt << std::endl;
						std::cout << "ppz cnt: " << uart_packet.data.cnt << std::endl;
						std::cout << "divergence: " << uart_packet.data.divergence << std::endl;
            			std::cout << "divergence_dot: " << uart_packet.data.divergence_dot << std::endl;
            			std::cout << "-----------" << std::endl;

						// WARNING: Only deep copying last valid message. Queue not created.
						ioctl(serial_port_fd_, TCFLSH, 0); // flush receive
						break;

					} else {
						std::cout << "[UART packet] Header and Footer not found" << std::endl;
						bytes_reverse_buf.pop_front();
					}
				}

				if (valid_uart_message_received) {
					// to parse only the lastest sbus message in the buffer of 4
					// Sometimes we read more than one sbus message at the same time
					// By running the loop above for as long as possible before handling
					// the received sbus message we achieve to only process the latest one
					// const SBusMsg received_sbus_msg = parseSbusMessage(sbus_msg_bytes);
					// handleReceivedSbusMessage(received_sbus_msg);
				}
			}
		}
	}

	return;
}

void SerialPort::serialPortTransmitThread() {
	std::cout << "[Serial] TransmitThread spawned" << std::endl;
	uint8_t buffer[10] = {0x24, 0x01, 0x08, 0x01, 0x01, 0x01, 0x01, 0x01, 0x0e, 0x2a};

	while(1) {
		static int var = 1;
		uart_packet_t uart_packet_tx = {0};

		uart_packet_tx.info.packet_type = DATA_FRAME;
		uart_packet_tx.info.packet_length = 0x08; // 1 start byte, 2 info bytes, 5 data bytes
		uart_packet_tx.data.pot = var;
		var = var + 1;

		// calc checksum
		uint8_t checksum = 0;
		uint8_t *p = (uint8_t *) &uart_packet_tx;
		for (int i = 0; i < sizeof(uart_packet_t); i++) {
    		checksum += p[i];
		}

		#ifdef DBG
			printf("\n\nchecksum: 0x%02x\n", checksum);
		#endif

		memcpy(&buffer[1], &uart_packet_tx, sizeof(uart_packet_t));
		memcpy(&buffer[8], &checksum, sizeof(uint8_t));

		#ifdef DBG
		for (int i = 0; i < sizeof(uart_packet_t) + 3; i++) {
			printf("0x%02x, ", buffer[i]);
		}
		#endif

		const int written = write(serial_port_fd_, buffer, uartFrameLength_ + 3);
  		// tcflush(serial_port_fd_, TCOFLUSH); // might not work on odroid
  		if (written != uartFrameLength_ + 3) {
			std::cout << "[Serial] written wrong bytes" << std::endl;
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

bool SerialPort::disconnectSerialPort() {

	// stop receiver thread
	if (!receiver_thread_.joinable()) {
    		return true;
  	}

	// atomic bool immediate stop,
	// stop parsing incorrect values at the end
	receiver_thread_should_exit_ = true;

  	// Wait for receiver thread to finish
  	receiver_thread_.join();

  	std::cout << "disconnected Serial Port" << std::endl;
  	close(serial_port_fd_);

}
