#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t* threads;
int global_index = 0;;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* thread_serach(void* args){
    pthread_mutex_lock(&mutex);

    int* array = (int*) args;
    int i, j, n, max, found_key = -1;
    n = array[0]; //can't guarantee that all array sizes will be same  (if our num workers doesnt divide)
    j = array[1]; //starting index will help us keep track where the hidden keys are
    max = -1;
    //printf("size of mini array: %d\n", n);
    // global_index++;
    for (i = 2; i < n+2; i++, j++){ //n+2 because we need to account for the first 2 extra elements
        if (array[i] > max)
            max = array[i];
        if (array[i] == -50){ //hidden key found
            printf("hidden key found at %d!\n", j);
            found_key = j;
        }
    }

    pthread_mutex_unlock(&mutex);
    int* return_vals = (int*) malloc(sizeof(int) * 2);
    return_vals[0] = max;
    //printf("found key result = %d\n", found_key);
    return_vals[1] = found_key;
    return (void*) return_vals;
}

int main(int argc, char* argv[]){ 
    //global_index = 0;
    //if argc, that's how many threads 
    
    time_t start,end;
    int size = 20, piece_size = 10; //TODO THIS WILL CHANGE

    int numWorkers, i,j,k,n;
    FILE* fp = fopen("test.txt",  "r"); //TODO this will change
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
    int maximums[numWorkers];
    int finalMax = -1, max_threadID = 0;

    //start timing
    time(&start);
    
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
    }
    //join and get absolute max
    for (i = 0; i < numWorkers; i++){
        //join and get return vals        
        int* return_vals = (int*) malloc(sizeof(int) * 2);

        pthread_join(threads[i], (void*) &return_vals);
        maximums[i] = (int) return_vals[0];
        printf("thread %u has return values of %d, and %d\n", &threads[i], (int) return_vals[0], (int) return_vals[1]); 
        if (maximums[i] > finalMax){
            finalMax = maximums[i];
            max_threadID = &threads[i];
        }
    }

    time(&end);
    fclose(fp);

    printf("The final maximum is %d, discovered by thread %u\ntime of execution: %f\n", finalMax, max_threadID, (double) (end-start)/CLOCKS_PER_SEC);

    


}


