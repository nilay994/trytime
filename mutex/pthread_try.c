/**
 * Emulates an ADC-SDcard scenario, where the sampling rate of ADC >> writing rate on a
 * low speed SPI bus. Handled by writing samples into the sdcard in chunks.
 */


#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

FILE* logfile;
#define MAX_SAMPLES 20

// global resource
float time_stamps = 0.0f;
int curr_sample = 0u;
float buffer[MAX_SAMPLES];

void* producer(void *ptr);
void* sampler(void* ptr);
void writer();

#define TICK_RATE     10000 // 10 ms
#define SAMPLING_RATE 10000 // 10 ms
#define WRITE_DELAY   50000 // 50 ms

void* producer(void *ptr)
{
    while (1) {
        // increment time stamp every 10 ms
        usleep(TICK_RATE);
        time_stamps += 0.01f;
    }
}

void* sampler(void* ptr)
{
    while(1) {
        usleep(SAMPLING_RATE);
        buffer[curr_sample++] = time_stamps;
        if (curr_sample >= MAX_SAMPLES) {
            writer();
            curr_sample = 0u;
        }
    }
}

// assume writer takes 50 ms to write to the memory
void writer()
{
    for (int i = 0; i < MAX_SAMPLES; i++) {
        fprintf(logfile, "%f\n", buffer[i]);
    }
    fflush(logfile);
    usleep(WRITE_DELAY);
}


void main()
{
    logfile = fopen("thread_try.csv", "w+");
    pthread_t thread1, thread2, thread3;
    
    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, sampler, NULL);
    // pthread_create(&thread3, NULL, writer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    // pthread_join(thread3, NULL); 

    fclose(logfile);
}