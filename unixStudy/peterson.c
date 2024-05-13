// a demo about peterson algorithm

// there are 2 process
#define N 2

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef int bool;

// the turn variable indicates which process can obtain the lock.
int turn = 0;
// thr flag array indicates which process want to obtain the lock.
// flag[0] = 1 : the process want to be locked when process_id = 0
bool flag[N] = {0, 0};

// process id
int process_ids[N] = {0, 1};

// lock function
// process_id: the process id that needs to be locked.
void lock(int process_id)
{
    int other_process_id = 1 - process_id;
    // current process id want to be locked;
    flag[process_id] = 1;
    turn = process_id;
    // the core of the peterson algorithm.
    while (turn == process_id && flag[other_process_id])
        ;
}

// unlock function
// process_id: the process want to release the lock.
void unlock(int process_id)
{
    flag[process_id] = 0;
}

void *process(void *arg)
{
    int process_id = *(int *)arg;
    // Enter the critical section, we need to lock it at this time.
    lock(process_id);

    // the critical section
    printf("Process %d is in critical section\n", process_id);

    // Leaving the critical section, the lock needs to be released.
    unlock(process_id);

    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[N];

    for (int i = 0; i < N; i++)
    {
        pthread_create(&threads[i], NULL, process, &process_ids[i]);
    }

    // wait for the process to end
    for (int i = 0; i < N; i++)
    {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
