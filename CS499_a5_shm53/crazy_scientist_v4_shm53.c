#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h> 
#include <omp.h>

#define SIZE 50

void *do_work(void *args);
double do_crazy_computation(int x, int y), mat[SIZE][SIZE], tstart, tend_1, tend_2;
pthread_mutex_t mutex;
int i = -1; //set to -1 since we will immediately increment it to 0 with the first thread

// boolean variable that will determine whether or not to keep looping in do_work
bool fifty_times = false; 

int main(int argc, char **argv)
{
    pthread_t thread_1, thread_2;
    pthread_mutex_init(&mutex, NULL);

    int thread_id[] = {0, 1};
    int *t1_ptr = thread_id;
    
    double tstart = omp_get_wtime();

    // set number of threads to 2
    // start both threads running
    pthread_create(&thread_1, NULL, do_work, (void *)(t1_ptr));
    pthread_create(&thread_2, NULL, do_work, (void *)(t1_ptr + 1));

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);

    double tend = omp_get_wtime();
    
    double elapsed = tend - tstart;
    double elapsed_t1 = tend_1 - tstart;
    double elapsed_t2 = tend_2 - tstart;
    double imbalance;
    
    // make it so that 'imbalance' is always positive
    if(elapsed_t1 > elapsed_t2)
    {
        imbalance = (elapsed_t1) - (elapsed_t2);
    }
    else
    {
        imbalance = (elapsed_t2) - (elapsed_t1);
    }

    printf("Total time (sanity check): %f seconds\n", elapsed);
    printf("Time Thread1: %f seconds\n", elapsed_t1);
    printf("Time Thread2: %f seconds\n", elapsed_t2);
    printf("Load imbalance: %f seconds\n", imbalance);

    exit(0);
}

//Crazy computation
void *do_work(void *args) 
{
    tstart = omp_get_wtime();
    int j, row;

    while (!fifty_times)
    { 
        pthread_mutex_lock(&mutex);
        row = i + 1;
        i++;

        if(i == SIZE)
        {
            fifty_times = true; 
        }
        pthread_mutex_unlock(&mutex);

        for (j = 0; j < SIZE; j++) /* loop over the columns */
        {  
            mat[row][j] = do_crazy_computation(row, j);
            fprintf(stderr, ".");
        }
    }

    if(*(int *)args == 0)
    {
        tend_1 = omp_get_wtime();
    }
    else
    {
        tend_2 = omp_get_wtime();
    }    
}

//Crazy computation
double do_crazy_computation(int x, int y) 
{
    int iter;
    double value = 0.0;

    for (iter = 0; iter < 5*x*x*x+1 + y*y*y+1; iter++) 
    {
        value += (cos(x*value) + sin(y*value));
    }
    return value;
}
