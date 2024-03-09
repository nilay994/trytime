/**
 * file: consumer.cpp
 * brief: Reads a new number from an existing ringbuffer
 *
 * TODO: come up with a maximum capacity based on the r/w rates in the rb
 */

#include <chrono>
#include <thread>
#include <iostream>
#include "consumer.h"
using namespace std;

void consumer(RingBuffer<uint8_t, 20> * rb)
{
    uint8_t new_spawn;
    cout << "Consumer created \n" << endl;
    FILE * fp = fopen("consumer.csv", "w+");
    while(1) {
        if (rb->read(&new_spawn)) {
            cout << "Consumer recieved: " << (int) new_spawn << endl;
            fprintf(fp, "%d\n", new_spawn);
            fflush(fp);
        }
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    fclose(fp);
}
