// Starts two threads, a producer and a consumer

#include <iostream>
#include <thread>
#include <stdint.h>
#include "RingBuffer.hpp"
#include "producer.h"
#include "consumer.h"


int main(int argv, char** argc)
{
	// common shared ringbuffer (not lockfree)
	RingBuffer<uint8_t, 20> rb;

	std::thread producer_thread(producer, &rb);
	std::thread consumer_thread(consumer, &rb);

	std::cout << "threads created!\n" << std::endl;

	producer_thread.join();
	consumer_thread.join();

	return 0;
}
