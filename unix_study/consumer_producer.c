// consumer-producer problem: Producers produce data for consumption by consumers.

// shared memory size for producers and consumers.
#define DEFAULT_BUFFER_SIZE 10

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

typedef int data;

// shared data area.
data buffer[DEFAULT_BUFFER_SIZE];

// in: an empty slot in the buffer.
// out: a full slot in the buffer.
// the buffer is empty when in == out
// the buffer is full when (in + 1) % DEFAULT_BUFFER_SIZE == out
int in = 0;
int out = 0;

void producer_process()
{
    srand(time(NULL));

    while (1)
    {

        while (((in + 1) % DEFAULT_BUFFER_SIZE) == out)
            ; // do nothing when buffer is full

        // produce some data
        data nextProcuced = rand();
        printf("produce a data, index: %d - data: %d .\n", in, nextProcuced);
        sleep(1);
        buffer[in] = nextProcuced;

        in = (in + 1) % DEFAULT_BUFFER_SIZE;
    }
}

void consumer_process()
{
    while (1)
    {

        while (in == out)
            ; // do nothing when buffer is empty

        // consume data
        data nextConsumed = buffer[out];

        // handler the data
        sleep(2);

        printf("consumer a data, index: %d - data: %d .\n", out, nextConsumed);
        out = (out + 1) % DEFAULT_BUFFER_SIZE;
    }
}

int main()
{
    // declare two threads to execute producer_process() and consumer_process()
    pthread_t producer_thread, consumer_thread;

    // create producer thread
    if (pthread_create(&producer_thread, NULL, (void *)producer_process, NULL) != 0)
    {
        perror("Error creating producer thread\n");
        exit(-1);
    }

    // create consumer thread
    if (pthread_create(&consumer_thread, NULL, (void *)consumer_process, NULL) != 0)
    {
        perror("Error creating consumer thread\n");
        exit(-1);
    }

    // wait for the thread to end
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);
    return 0;
}
