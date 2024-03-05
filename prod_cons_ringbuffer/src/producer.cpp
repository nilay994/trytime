/**
 * file: producer.cpp
 * brief: Adds a new number to an existing ringbuffer
 */

#include <chrono>
#include <thread>
#include <iostream>
#include "producer.h"

using namespace std;

static uint8_t new_spawn = 0u;

void producer(RingBuffer<uint8_t, 20> * rb)
{
    cout << "Producer created \n" << endl;
    FILE * fp = fopen("producer.csv", "w+");
    while (1) {
        new_spawn += 1u;
        rb->write(new_spawn);
        rb->display_status();
        fprintf(fp, "%d\n", new_spawn);
	    this_thread::sleep_for(chrono::milliseconds(300));
        fflush(fp);
    }
    fclose(fp);
}
