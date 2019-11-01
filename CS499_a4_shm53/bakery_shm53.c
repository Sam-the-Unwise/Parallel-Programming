#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>

void random_sleep(int a, int b);
void *lh_bake(void *arg);
void *rh_bake(void *arg);
void *c_bake(void *arg);

#define NUM_ITERATIONS 10
#define NUM_LEFT_OVEN_MITTS 3
#define NUM_RIGHT_OVEN_MITTS 3

#define MINIMUM_SECONDS_OF_SLEEP 2
#define MAXIMUM_SECONDS_OF_SLEEP 5


typedef struct neededInfo{
    
    int baker_number;
    sem_t *left_ovenmits;
    sem_t *right_ovenmits;

}neededInfo;

/* Helper function to sleep a random number of microseconds
 * picked between two bounds (provided in seconds)
 */
void random_sleep(int lbound_sec, int ubound_sec) 
{
   int num_usec;
   num_usec = lbound_sec*100000 + 
	      (int)((ubound_sec - lbound_sec)*100000 * ((double)(rand()) / RAND_MAX)); 
   usleep(num_usec);
   return; 
}

/*
 * Main function
 */
int main(int argc, char **argv) 
{
    int num_left_handed_bakers, num_right_handed_bakers, num_cautious_bakers;
    int seed, create_lhb, create_rhb, create_cb;
    int end_lhb, end_rhb, end_cb;
    sem_t left_mit, right_mit;

    // initialize semaphors
    sem_init(&right_mit, 0, NUM_LEFT_OVEN_MITTS);
    sem_init(&left_mit, 0, NUM_RIGHT_OVEN_MITTS);

    pthread_mutex_t l_mutex, r_mutex;

    // Process command-line arguments 
    if (argc != 5) 
    {
        fprintf(stderr,"Usage: %s <# left-handed bakers> <# right-handed bakers> <# cautious bakers> <seed>\n",argv[0]);
        exit(1);
    }

    // make sure all values at least 1
    if ((sscanf(argv[1],"%d", &num_left_handed_bakers) != 1) || 
        (sscanf(argv[2],"%d", &num_right_handed_bakers) != 1) || 
        (sscanf(argv[3],"%d", &num_cautious_bakers) != 1) || 
        (sscanf(argv[4],"%d", &seed) != 1) ||
        (num_left_handed_bakers < 1) ||
        (num_right_handed_bakers < 1) ||
        (num_cautious_bakers < 1) ||
        (seed < 0)) 
    {
        fprintf(stderr,"Invalid command-line arguments... Aborting\n");
        exit(1);
    }
    
    /* Seed the RNG */
    srand(seed);

    pthread_t lh_bakers[num_left_handed_bakers], 
                rh_bakers[num_right_handed_bakers], 
                c_bakers[num_cautious_bakers];

    struct neededInfo *info[num_left_handed_bakers + num_right_handed_bakers + num_cautious_bakers];

    // IMPLEMENT CODE HERE

    // CREATE THREADS
    for(create_lhb = 0; create_lhb < num_left_handed_bakers; create_lhb++)
    {
        info[create_lhb] = (neededInfo *)malloc(sizeof(neededInfo));
        //create struct that will hold the info our thread needs
        info[create_lhb]->baker_number = create_lhb;
        info[create_lhb]->right_ovenmits = &right_mit;
        info[create_lhb]->left_ovenmits = &left_mit;

        pthread_create(&lh_bakers[create_lhb], NULL, lh_bake, (void *)(info[create_lhb]));
    }

    for(create_rhb = 0; create_rhb < num_right_handed_bakers; create_rhb++)
    {
        info[num_left_handed_bakers + create_rhb] = (neededInfo *)malloc(sizeof(neededInfo));

        //create struct that will hold the info our thread needs
        info[num_left_handed_bakers + create_rhb]->baker_number = create_rhb;
        info[num_left_handed_bakers + create_rhb]->right_ovenmits = &right_mit;
        info[num_left_handed_bakers + create_rhb]->left_ovenmits = &left_mit;

        pthread_create(&rh_bakers[create_rhb], NULL, rh_bake, (void *)(info[num_left_handed_bakers + create_rhb]));
    }

    for(create_cb = 0; create_cb < num_cautious_bakers; create_cb++)
    {
        info[num_left_handed_bakers + num_right_handed_bakers + create_cb] = (neededInfo *)malloc(sizeof(neededInfo));

        //create struct that will hold the info our thread needs
        info[num_left_handed_bakers + num_right_handed_bakers + create_cb]->baker_number = create_cb;
        info[num_left_handed_bakers + num_right_handed_bakers + create_cb]->right_ovenmits = &right_mit;
        info[num_left_handed_bakers + num_right_handed_bakers + create_cb]->left_ovenmits = &left_mit;

        pthread_create(&c_bakers[create_cb], NULL, c_bake, (void *)(info[num_left_handed_bakers + num_right_handed_bakers + create_cb]));
    }


    // JOIN THREADS
    for(end_lhb = 0; end_lhb < num_left_handed_bakers; end_lhb++)
    {
        pthread_join(lh_bakers[end_lhb], NULL);
        free(info[end_lhb]);
    }

    for(end_rhb = 0; end_rhb < num_right_handed_bakers; end_rhb++)
    {
        pthread_join(rh_bakers[end_rhb], NULL);
        free(info[num_left_handed_bakers + end_rhb]);
    }

    for(end_cb = 0; end_cb < num_cautious_bakers; end_cb++)
    {
        pthread_join(c_bakers[end_cb], NULL);
        free(info[num_left_handed_bakers + num_right_handed_bakers + end_cb]);
    }

    exit(0);
}


void *lh_bake(void *arg)
{
    int baker_num = (((neededInfo *)arg)->baker_number);
    
    // make each baker work for specified number of times
    for(int turn = 0; turn < NUM_ITERATIONS; turn++)
    {
        printf("[Left-handed baker %d] is working...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Left-handed baker %d] wants a left-handed mitt...\n", baker_num);
        //get oven mit
        sem_wait(((neededInfo *)arg)->left_ovenmits);
        printf("[Left-handed baker %d] has got a left-handed mitt...\n", baker_num);

        printf("[Left-handed baker %d] has put cookies in the oven and is waiting...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Left-handed baker %d] has taken cookies out of the oven...\n", baker_num);

        // put oven mitts back onto the oven mitt rack
        sem_post(((neededInfo *)arg)->left_ovenmits);
        printf("[Left-handed baker %d] has put back a left-handed mitt...\n", baker_num);
    }
}

void *rh_bake(void *arg)
{
    int baker_num = (((neededInfo *)arg)->baker_number);

    // make each baker work for specified number of times
    for(int turn = 0; turn < NUM_ITERATIONS; turn++)
    {
        printf("[Right-handed baker %d] is working...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Right-handed baker %d] wants a right-handed mitt...\n", baker_num);
        
        // get oven mit
        sem_wait(((neededInfo *)arg)->right_ovenmits);
        printf("[Right-handed baker %d] has got a right-handed mitt...\n", baker_num);

        printf("[Right-handed baker %d] has put cookies in the oven and is waiting...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Right-handed baker %d] has taken cookies out of the oven...\n", baker_num);

        // put oven mitts back onto the oven mitt rack
        sem_post(((neededInfo *)arg)->right_ovenmits);
        printf("[Right-handed baker %d] has put back a right-handed mitt...\n", baker_num);
    }
}

void *c_bake(void *arg)
{
    int baker_num = (((neededInfo *)arg)->baker_number);

    // make each baker work for specified number of times
    for(int turn = 0; turn < NUM_ITERATIONS; turn++)
    {
        printf("[Cautious baker %d] is working...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Cautious baker %d] wants a left-handed mitt...\n", baker_num);

        sem_wait(((neededInfo *)arg)->left_ovenmits);
        printf("[Cautious baker %d] has got a left-handed mitt...\n", baker_num);
        printf("[Cautious baker %d] wants a right-handed mitt...\n", baker_num);
        
        sem_wait(((neededInfo *)arg)->right_ovenmits);
        printf("[Cautious baker %d] has got a right-handed mitt...\n", baker_num);

        printf("[Cautious baker %d] has put cookies in the oven and is waiting...\n", baker_num);
        random_sleep(MINIMUM_SECONDS_OF_SLEEP, MAXIMUM_SECONDS_OF_SLEEP);

        printf("[Cautious baker %d] has taken cookies out of the oven...\n", baker_num);

        sem_post(((neededInfo *)arg)->left_ovenmits);
        printf("[Cautious baker %d] has put back a left-handed mitt...\n", baker_num);
        
        sem_post(((neededInfo *)arg)->right_ovenmits);
        printf("[Cautious baker %d] has put back a right-handed mitt\n", baker_num);
    }
}
