/* Sync between producer consumer threads
 *
 * Let the sensors spawn new data at whichever rates they can.
 * Let the control loops use the data at a similar rate.
 * Synchronize both threads using a lock-free/wait-free data structure.
 *
 * Boost SPSC is however, not a ringbuffer.
 * refer: Jordan Restifo, AlphaPilot 2019
 *
 */

// maybe use a queue when multiple consumers
// #include <queue>
#include <iostream>
#include <stdint.h>
#include <thread>
#include <chrono>

#include <boost/lockfree/spsc_queue.hpp>
#include <boost/lockfree/queue.hpp>

struct uart_buffer {
	float val_f32;
	uint8_t val_u8;
};

// global queue
boost::lockfree::spsc_queue<uart_buffer, boost::lockfree::capacity<8192>> _buffer_queue;

void producer() {
	uart_buffer buffer_data = {0};
	while(1) {
		buffer_data.val_f32 = buffer_data.val_f32 + 0.1f;

		if (!_buffer_queue.push(buffer_data)) {
			std::cout << "[err] producer failed to push buffer data" << std::endl;
			_buffer_queue.reset();
		}

		std::cout << "producer: write available: " << _buffer_queue.write_available() << std::endl;

		// also print the queue?
		std::this_thread::sleep_for(std::chrono::milliseconds(4));
	}
}

void consumer() {
	uart_buffer buffer_data;
	while(1) {
		if (!_buffer_queue.pop(buffer_data)) {
			std::cout << "[err] consumer failed to get buffer data" << std::endl;
		}

		std::cout << "consumer: read available: " << _buffer_queue.read_available() << std::endl;

		std::cout << "val_f32: " << buffer_data.val_f32 << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}
}



int main(int argv, char** argc) {

	std::thread thread1(producer);
	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	std::thread thread2(consumer);

	std::cout << "threads created!" << std::endl;

	thread1.join();
	thread2.join();

	return 0;
}
