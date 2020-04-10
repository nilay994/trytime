/* 
Threaded UART receiving buffer for any linux computer
Adopted from: 
        MIT License Copyright (c) 2018 Matthias Faessler (Robotics and Perception Group, 
        University of Zurich, Switzerland)
Modified by:
        nilay994
*/

#include <thread>
#include <atomic>
#include "uart_struct.h"

#define FRAMELEN 7

class SerialPort {
        public:
                SerialPort(const std::string& port);
                ~SerialPort();
                void serialPortReceiveThread();
                uint8_t uart_msg_bytes[FRAMELEN];
                bool valid_uart_message_received = false;

        private:
                bool connectSerialPort(const std::string& port);
                bool configSerialPort() const;
                bool disconnectSerialPort();
                bool startReceiverThread();
                // serial port int handle
                int serial_port_fd_;
                std::thread receiver_thread_;
                std::atomic_bool receiver_thread_should_exit_;

                static constexpr uint8_t HeaderByte_ = 0x24;
                static constexpr uint8_t FooterByte_ = 0x2A;
                static constexpr int kPollTimeoutMilliSeconds_ = 500;
                static constexpr int uartFrameLength_ = FRAMELEN;

};