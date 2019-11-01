#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 3

// declaration of functions
void *do_work(void *arg);

int curr_index, buffer[SIZE];
pthread_mutex_t mutex;
int count = 0, sequence_count = 0;

/******************************************************************************
 * FUNCTION: main
 * DESCRIPTION: handles our threads, mutex lock, and all other happenings of 
 *              our function
 * RETURN: error code if mishap, 0 if no mishap
******************************************************************************/

int main(int argc, char **argv)
{
    pthread_t thread_1, thread_2, thread_3;
    int numbers[] = {1, 2, 3};
    int *nums = numbers;

    pthread_mutex_init(&mutex, NULL);

    if(pthread_create(&thread_1, NULL, do_work, (void *)(nums)))
    {
        fprintf(stderr, "Error while creating thread 1\n");
        exit(1);
    }

    if(pthread_create(&thread_2, NULL, do_work, (void *)(nums + 1)))
    {
        fprintf(stderr, "Error while creating thread 2\n");
        exit(1);
    }

    if(pthread_create(&thread_3, NULL, do_work, (void *)(nums + 2)))
    {
        fprintf(stderr, "Error while creating thread \n");
        exit(1);
    }

    if(pthread_join(thread_1, NULL))
    {
        fprintf(stderr, "Error while joining with child thead 1\n");
        exit(1);
    }

    if(pthread_join(thread_2, NULL))
    {
        fprintf(stderr, "Error while joining with child thead 2\n");
        exit(1);
    }

    if(pthread_join(thread_3, NULL))
    {
        fprintf(stderr, "Error while joining with child thead 3\n");
        exit(1);
    }

    printf("Total sequences generated: %d\n", sequence_count);
    printf("Number of correct sequences: %d\n", count);

    exit(0);
}

/******************************************************************************
 * FUNCTION:
 * DESCRIPTION:
 * RETURN: none
******************************************************************************/

void *do_work(void *arg)
{    
    //lock so other threads can't mess with variables
    pthread_mutex_lock(&mutex);
    int curr_int = *(int *)arg;

    while(count < 10)
    {
        printf("My id: %d\n", curr_int);
        buffer[curr_index] = curr_int;
        curr_index ++;

        if(curr_index == 3)
        {
            if(buffer[0] == 1 && buffer[1] == 2 && buffer[2] == 3)
            {
                printf("123\n");
                count ++;
            }

            sequence_count ++;
            curr_index = 0;
        }

        pthread_mutex_unlock(&mutex); // unlock for other threads

        usleep(500000);
    }
}