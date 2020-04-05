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


SerialPort::SerialPort(const std::string &port)
	: receiver_thread_(),
	  receiver_thread_should_exit_(false),
	  serial_port_fd_(-1) {

	if (!connectSerialPort(port)) {
		std::cout << "[ERR] can't connect to port" << std::endl;
	}

	if (!startReceiverThread()) {
		std::cout << "[ERR] can't start receiver thread" << std::endl;
	}
}

SerialPort::~SerialPort() { disconnectSerialPort(); }

bool SerialPort::connectSerialPort(const std::string &port) {
	// Open serial port
	// O_RDWR - Read and write
	// O_NOCTTY - Ignore special chars like CTRL-C
	serial_port_fd_ = open(port.c_str(), O_RDWR | O_NOCTTY);

	if (serial_port_fd_ == -1) {
		std::cout << "[ERR] can't open serial" << std::endl;
		return false;
	}
	if (!configSerialPort()) {
		close(serial_port_fd_);
		std::cout << "[ERR] can't config serial" << std::endl;
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

bool SerialPort::configSerialPort() const {
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
	//
	uart_config.c_oflag &= ~(OCRNL | ONLCR | ONLRET | ONOCR | OFILL | OPOST);

	// Input flags - Turn off input processing
	// convert break to null byte, no CR to NL translation,
	// no NL to CR translation, don't mark parity errors or breaks
	// no input parity check, don't strip high bit off,
	// no XON/XOFF software flow control
	//
	uart_config.c_iflag &=
		~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);

	//
	// No line processing:
	// echo off
	// echo newline off
	// canonical mode off,
	// extended input processing off
	// signal chars off
	//
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
	const speed_t spd = 115200;
	uart_config.c_ispeed = spd;
	uart_config.c_ospeed = spd;

	// error in making settings
	if (ioctl(serial_port_fd_, TCSETS2, &uart_config) < 0) {
		return false;
	}
	std::cout << "configured serial! " << std::endl;
	return true;
}

void SerialPort::serialPortReceiveThread() {

	struct pollfd fds[1];
	fds[0].fd = serial_port_fd_;
	fds[0].events = POLLIN;
	std::cout << "serialPortReceiveThread spawned" << std::endl;
	// init of thread
	uint8_t init_buf[10];
	while (read(serial_port_fd_, init_buf, sizeof(init_buf)) > 0) {
		// On startup, as long as we receive something, we keep reading to ensure
		// that the first byte of the first poll is the start of an SBUS message
		// and not some arbitrary byte.
		// This should help to get the framing in sync in the beginning.
		usleep(100);
	}

	std::deque<uint8_t> bytes_buf;

	// while atomic lock of thread
	while (!receiver_thread_should_exit_) {
		// Buffer to read bytes from serial port. We make it large enough to
		// potentially contain 4 sbus messages but its actual size probably does
		// not matter too much
		// TODO: 4 struct size
		uint8_t read_buf[4 * uartFrameLength_];

		if (poll(fds, 1, kPollTimeoutMilliSeconds_) > 0) {
			if (fds[0].revents & POLLIN) {
				const ssize_t nread = read(serial_port_fd_, read_buf, sizeof(read_buf));

				for (ssize_t i = 0; i < nread; i++) {
					bytes_buf.push_back(read_buf[i]);
				}

				valid_uart_message_received = false;			
				memset(uart_msg_bytes, 0, uartFrameLength_);

				while (bytes_buf.size() >= uartFrameLength_) {
					// Check if we have a potentially valid SBUS message
					// A valid SBUS message must have to correct header and footer byte
					// as well as zeros in the four most significant bytes of the flag
					// byte (byte 23)
					if (bytes_buf.front() == HeaderByte_ && 
							bytes_buf[uartFrameLength_ - 1] == FooterByte_) {
						for (uint8_t i = 0; i < uartFrameLength_; i++) {
							uart_msg_bytes[i] = bytes_buf.front();
							bytes_buf.pop_front();
							// printf("0x%02x,", uart_msg_bytes[i]);
						}
						// std::cout << "works" << std::endl;
						valid_uart_message_received = true;
					} else {
						// If it is not a valid SBUS message but has a correct header byte
						// we need to pop it to prevent staying in this loop forever
						bytes_buf.pop_front();
						// warn, not in sync
					}

					// If not, pop front elements until we have a valid header byte
					while (!bytes_buf.empty() && bytes_buf.front() != HeaderByte_) {
						bytes_buf.pop_front();
						std::cout<< "dframe aligning" << std::endl;
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
