#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#define SIZE 3

// declaration of functions
void *do_work(void *arg);
void *do_work2(void *arg);


struct numbers {
    int num_to_print;
};

int curr_index, buffer_1[SIZE], buffer_2[SIZE], index_1, index_2;
int team_1 = 0, team_2 = 0, sequence_count = 0, team_1_total = 0, team_2_total = 0;
pthread_mutex_t mutex_1, mutex_2;
pthread_cond_t won;

/******************************************************************************
 * FUNCTION: main
 * DESCRIPTION: handles our threads, mutex lock, and all other happenings of 
 *              our function -- specifically will call do_work and do_work2 in
 *              order to print out threads 1-6. A race between 1-3 and 4-6 will
 *              occur, and the first to print out their respective numbers in
 *              order 10 times will win
 * RETURN: error code if mishap, 0 if no mishap
******************************************************************************/

int main(int argc, char **argv)
{
    pthread_t thread_1, thread_2, thread_3, thread_4, thread_5, thread_6;
    int numbers[] = {1, 2, 3, 4, 5, 6};
    int *nums = numbers;

    pthread_mutex_init(&mutex_1, NULL);
    pthread_mutex_init(&mutex_2, NULL);

    index_1 = 0;
    index_2 = 0;

    // CREATING THREADS
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

    if(pthread_create(&thread_4, NULL, do_work2, (void *)(nums + 3)))
    {
        fprintf(stderr, "Error while creating thread 1\n");
        exit(1);
    }

    if(pthread_create(&thread_5, NULL, do_work2, (void *)(nums + 4)))
    {
        fprintf(stderr, "Error while creating thread 2\n");
        exit(1);
    }

    if(pthread_create(&thread_6, NULL, do_work2, (void *)(nums + 5)))
    {
        fprintf(stderr, "Error while creating thread \n");
        exit(1);
    }

    // JOINING THREADS
    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);
    pthread_join(thread_4, NULL);
    pthread_join(thread_5, NULL);
    pthread_join(thread_6, NULL);


    if(team_1 == 10)
    {
        printf("Team 1 won!\n");
        
    }
    else
    {
        printf("Team 2 won!\n");
    }

    printf("Total sequences generated team1: %d\n", team1_total);
    printf("Number of correct sequences team1: %d\n", team_1);
    printf("Total sequences generated team2: %d\n", team2_total);
    printf("Number of correct sequences team2: %d\n", team_2);
    

    
    exit(0);
}


/******************************************************************************
 * FUNCTION: do_work
 * DESCRIPTION: function called by threads 1, 2, 3 that will print out their
 *              respective number to the screen and save the numbers to a buffer
 *              in the order that they are printed out
 * RETURN: none
******************************************************************************/

void *do_work(void *arg)
{
    bool proceed_1 = true;

    pthread_mutex_lock(&mutex_2);

    if(team_2 == 10)
    {
        proceed_1 = false;
    }

    pthread_mutex_unlock(&mutex_2);


    //lock so no one can mess with variables
    pthread_mutex_lock(&mutex_1);

    // if(buffer_1[0] == 1 && buffer_1[1] == 2 && buffer_1[2] == 3)
    // {
    //     printf("123\n");
    //     team_1 ++;
    // }

    if(proceed_1 == true)
    {
        printf("My id: %d\n", *(int *)arg);
        buffer_1[index_1] = *(int *)arg;
        index_1 ++;
    }

    team_1_total ++;

    pthread_mutex_unlock(&mutex_1); // unlock for other threads
    
    usleep(5000);
}


/******************************************************************************
 * FUNCTION: do_work2
 * DESCRIPTION: function called by threads 4, 5, 6 that will print out their
 *              respective number to the screen and save the numbers to a buffer
 *              in the order that they are printed out
 * RETURN: none
******************************************************************************/

void *do_work2(void *arg)
{
    bool proceed_2 = true;

    pthread_mutex_lock(&mutex_1);

    if(team_1 == 10)
    {
        proceed_2 = false;
    }

    pthread_mutex_unlock(&mutex_1);


    //lock so no one can mess with variables
    pthread_mutex_lock(&mutex_2);

    // if(buffer_2[0] == 4 && buffer_2[1] == 5 && buffer_2[2] == 6)
    // {
    //     printf("456\n");
    //     team_2 ++;
    // }

    if(proceed_2 == true)
    {
        printf("My id: %d\n", *(int *)arg);
        buffer_2[index_2] = *(int *)arg;
        index_2 ++;
    }

    team_2_total ++;

    pthread_mutex_unlock(&mutex_2); // unlock for other threads

    usleep(5000);
}