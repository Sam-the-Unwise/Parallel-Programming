#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *increment_work(void *arg);
void *decrement_work(void *arg);

pthread_mutex_t mutex;
pthread_cond_t count_not_zero;
pthread_cond_t count_not_ten;
int count, inc_count, dec_count;

/******************************************************************************
 * FUNCTION: main
 * DESCRIPTION: handles our threads, mutex lock, and all other happenings of 
 *              our function
 * RETURN: error code if mishap, 0 if no mishap
******************************************************************************/

int main(int argc, char **argv)
{
    pthread_t inc_thread, dec_thread;
    count = 0, inc_count = 0, dec_count = 0;

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&count_not_zero, NULL);
    pthread_cond_init(&count_not_ten, NULL);

    if(pthread_create(&inc_thread, NULL, increment_work, NULL))
    {
        fprintf(stderr, "Error while creating increment thread\n");
        exit(1);
    }

    if(pthread_create(&dec_thread, NULL, decrement_work, NULL))
    {
        fprintf(stderr, "Error while creating decrement thread\n");
        exit(1);
    }

    pthread_join(inc_thread, NULL);
    pthread_join(dec_thread, NULL);
}


/******************************************************************************
 * FUNCTION: increment_work
 * DESCRIPTION: will increment our variable 'count' until it reaches 10. If
 *              variale is higher than 0, will wait until decremented to 0
 * RETURN: none
******************************************************************************/

void *increment_work(void *arg)
{
    usleep(5000);

    pthread_mutex_lock(&mutex);

    while(inc_count != 3)
    {

        // hardcode in counter to keep loop from going above 30 increments
        while(count != 0)
        {
            //printf("inc count waiting: %d\n", count);
            pthread_cond_wait(&count_not_zero, &mutex);
        }
            
        while(count != 10)
        {
            count ++;
            printf("Count is now (inc fn): %d\n", count);
            pthread_cond_signal(&count_not_ten);
            
        }

        inc_count ++;
    }

    pthread_mutex_unlock(&mutex);
}

/******************************************************************************
 * FUNCTION: decrement_work
 * DESCRIPTION: will decrement our variable 'count' until it reaches 0. If
 *              variale is at 0, will wait until incremented to 10
 * RETURN: none
******************************************************************************/

void *decrement_work(void *arg)
{
    usleep(5000);

    pthread_mutex_lock(&mutex);

    while (dec_count != 2)
    {
        while(count != 10)
        {
            //printf("dec count waiting: %d\n", count);
            pthread_cond_wait(&count_not_ten, &mutex);
        }

        while(count != 0)
        {
            count --;
            printf("Count is now (dec fn): %d\n", count);
            pthread_cond_signal(&count_not_zero);
        }

        dec_count ++;
    }
 
    pthread_mutex_unlock(&mutex);
}