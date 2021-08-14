/**
 * Emulates an ADC-SDcard scenario, where the sampling rate of ADC >> writing rate on a
 * low speed SPI bus. Faster by writing samples into the sdcard in chunks?
 */
#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <string.h>

FILE* logfile;
#define MAX_SAMPLES 20

typedef struct {
    float t;
    int a;
    int b;
    int c;
} st_t;

// global resource
float time_stamps = 0.0f;
int curr_sample = 0u;
st_t buffer[MAX_SAMPLES] = {0u};
pthread_mutex_t buff_lock;

void* producer(void *ptr);
void* sampler(void* ptr);
void* writer(void *ptr);

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

// can hold SAMPLING_RATE*MAX_SAMPLES = 200ms of data
void* sampler(void* ptr)
{
    while(1) {
        usleep(SAMPLING_RATE);
        if (curr_sample == MAX_SAMPLES) {
            curr_sample = 0u;
        }
        pthread_mutex_lock(&buff_lock);
        buffer[curr_sample++].t = time_stamps;
        pthread_mutex_unlock(&buff_lock);
    }
}

// assume writer takes 50 ms to write to the memory
void* writer(void *ptr)
{
    st_t buffer_cp[MAX_SAMPLES];
    while (1) {
        if (curr_sample == MAX_SAMPLES) {
            pthread_mutex_lock(&buff_lock);
            for (int i = 0; i < MAX_SAMPLES; i++) {
                // sample-wise deep copy the struct array
                memcpy(&buffer_cp[i], &buffer[i], sizeof(st_t));
            }
            pthread_mutex_unlock(&buff_lock);
            // go ahead, take as much time as you want, but not
            // longer than MAX_SAMPLES * SAMPLING_RATE 
            for (int i = 0; i < MAX_SAMPLES; i++) {
                fprintf(logfile, "%f\n", buffer_cp[i].t);
            }
            fflush(logfile);
            usleep(WRITE_DELAY);
        }
    }
}


void main()
{
    logfile = fopen("thread_try.csv", "w+");
    pthread_t thread1, thread2, thread3;
    
    pthread_mutex_init(&buff_lock, NULL);
    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, sampler, NULL);
    pthread_create(&thread3, NULL, writer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL); 
    pthread_mutex_destroy(&buff_lock);

    fclose(logfile);
}