#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 10

// declaration of functions
void *do_work(void *arg);

struct numbers {
    int num_to_print;
};

int total = 0;
pthread_mutex_t mutex;

/******************************************************************************
 * FUNCTION: main
 * DESCRIPTION: handles our threads, mutex lock, and all other happenings of 
 *              our function
 * RETURN: error code if mishap, 0 if no mishap
******************************************************************************/

int main(int argc, char **argv)
{
    pthread_t thread[SIZE];
    int numbers[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *nums = numbers;
    int num_index, thread_index;

    pthread_mutex_init(&mutex, NULL);
    
    
    while (total != 990)
    {
        // generate all threads
        for(thread_index = 0; thread_index < SIZE; thread_index ++)
        {
            if(pthread_create(&thread[thread_index], NULL, do_work, 
                             (void *)(nums + thread_index)))
            {
                fprintf(stderr, "Error while creating thread 1\n");
                exit(1);
            }

            if(pthread_join(thread[thread_index], NULL))
            {
                fprintf(stderr, "Error while joining with child thead 1\n");
                exit(1);
            }
        }
    }

    printf("Total: %d\n", total);

    exit(0);
}

/******************************************************************************
 * FUNCTION: do_work
 * DESCRIPTION: simple function that adds up the current threads number to the
 *              total variable -- should only add up the threads in order (e.g.
 *              1, 2, 3, 4 and not 1, 4, 2, 3)
 * RETURN: none
******************************************************************************/

void *do_work(void *arg)
{
    usleep(5000);
    
    //lock so no one can mess with variables
    pthread_mutex_lock(&mutex);
    
    total += *(int *)arg;
    printf("my num: %d, total: %d\n", *(int *)arg, total);
 
    pthread_mutex_unlock(&mutex); // unlock for other threads
}