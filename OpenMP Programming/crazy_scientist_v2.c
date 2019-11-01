#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h> 

#define SIZE 50

double do_crazy_computation(int i, int j);

int main(int argc, char **argv) 
{
    double mat[SIZE][SIZE];
    int i, j, tid;

    double tend1=0.0, tend2=0.0;

    double tstart = omp_get_wtime();

    // set number of threads to 2
    omp_set_num_threads(2);

    //modify code here
    #pragma omp parallel private(i, j, tid) shared(tend1, tend2, mat)
    {
        tid = omp_get_thread_num();
        
        #pragma omp for schedule(static) nowait
        for (i = 0; i < SIZE; i++) 
        { /* loop over the rows */
            for (j = 0; j < SIZE; j++) 
            {  /* loop over the columns */
                mat[i][j] = do_crazy_computation(i,j);
                fprintf(stderr,".");
            }
        }

        //get time of certain thread based on thread
        if(tid == 0)
        {
            tend1 = omp_get_wtime();
            
        }
        else
        {
            tend2 = omp_get_wtime();
        }
    }

    double tend = omp_get_wtime();

    double elapsed = tend - tstart;
    double elapsed_t1 = tend1 - tstart;
    double elapsed_t2 = tend2 - tstart;
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
