#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t* threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int keys_found; 
void* thread_serach(void* args){
    
    pthread_mutex_lock(&mutex);

    int* array = (int*) args;
    int i, j, n, found_key = -1;
    n = array[0]; //can't guarantee that all array sizes will be same  (if our num workers doesnt divide)
    j = array[1]; //starting index will help us keep track where the hidden keys are
    
    //printf("size of mini array: %d\n", n);
    for (i = 2; i < n+2; i++, j++){ //n+2 because we need to account for the first 2 extra elements
       
        if (array[i] == -50){ //hidden key found
            printf("hidden key found at %d!\n", j);
            found_key = j;
            keys_found++;
        }
    }

    pthread_mutex_unlock(&mutex);
    
    int* return_vals = (int*) malloc(sizeof(int) );
    return_vals[0] = found_key;
    //printf("found key result = %d\n", found_key);
    return (void*) return_vals;
}

int main(int argc, char* argv[]){ 
    keys_found = 0;
    //if argc, that's how many threads 
    struct timeval start,end;
    int size = 20, piece_size = 10; //TODO THIS WILL CHANGE

    int numWorkers, i,j,k,n;
    FILE* fp = fopen("test.txt",  "r"); //TODO THIS WILL CHANGE
    if (fp == NULL) return -1;

    if (argc > 1){
        numWorkers = atoi(argv[1]);
        if (numWorkers > size) numWorkers = 20;
        for (i = 0; i < numWorkers; i++){
            //create threads 
            mini_array_size = ceil((double)size/numWorkers); 
        }
    }
    else{
        //divide by pieces size
        numWorkers = size/piece_size; 
        mini_array_size = 10;
    }
    
    threads = (pthread_t*) malloc(sizeof(pthread_t)*numWorkers);

    //start timing
    gettimeofday(&start,NULL);
    
    for (i = 0, k = 0; k < numWorkers; i+=j-2, k++){  //this is a little OD but it's fine
        pthread_t curr_thread;

        //create mini array
        int* mini_array = (int*) malloc(sizeof(int) * (mini_array_size + 2)); //adding 2 in order to add size and starting index
        for (j = 2; j < mini_array_size+2; j++){
            if (fscanf(fp,"%d\n", &n) == EOF) break;
            //printf("adding item %d to mini array\n", n);
            mini_array[j] = n;
        }

        mini_array[0] = j-2; //size of array
        mini_array[1] = i; //starting index
        //printf("i = %d, j = %d\n", i,j);
        //mini array done, time to thread
        pthread_create(&curr_thread, NULL, thread_serach, (void*) mini_array);
        threads[k] = curr_thread;
        free(mini_array);
    }
    //join and get absolute max
    for (i = 0; i < numWorkers; i++){
        
        //check if all keys have already been found //TODO here or in prev loop?
        if (keys_found == 3){
            break;
        }

        //join and get return vals        
        int* return_vals = (int*) malloc(sizeof(int));
        pthread_join(threads[i], (void*) &return_vals);
        int key_index = (int) return_vals[0];
        printf("thread %u has key index %d\n", &threads[i], (int) return_vals[0]); 
        
    }

    fclose(fp);

    //end timing
    gettimeofday(&end,NULL);
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
    printf("time of execution: %f usec\n", runTime);

    


}


