// Compile with `gcc prio-inv.c -lpthread`
// TODO: find out the scenario why this went wrong

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

// Shared resource protected by a mutex
int shared_resource = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

// Task A, with high priority
void* taskA(void* arg)
{
    while(1)
    {
        printf("Task A is running\n");

        // Acquire the shared resource
        pthread_mutex_lock(&mutex);
        shared_resource = 1;

        // Sleep for a while to simulate work
        usleep(100000);

        // Release the shared resource
        shared_resource = 0;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Task B, with medium priority
void* taskB(void* arg)
{
    while(1)
    {
        printf("Task B is running\n");

        // Acquire the shared resource
        pthread_mutex_lock(&mutex);
        shared_resource = 2;

        // Sleep for a while to simulate work
        usleep(100000);

        // Release the shared resource
        shared_resource = 0;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

// Task C, with low priority
void* taskC(void* arg)
{
    while(1)
    {
        printf("Task C is running\n");

        // Sleep for a while to simulate work
        usleep(100000);
    }

    return NULL;
}

int main()
{
    // Create threads for each task
    pthread_t threadA, threadB, threadC;
    pthread_create(&threadA, NULL, taskA, NULL);
    pthread_create(&threadB, NULL, taskB, NULL);
    pthread_create(&threadC, NULL, taskC, NULL);

    // Wait for threads to finish (which will never happen)
    pthread_join(threadA, NULL);
    pthread_join(threadB, NULL);
    pthread_join(threadC, NULL);

    return 0;
}
