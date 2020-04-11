#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t* threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int keys_found = 0; 

void* thread_serach(void* args){

   if (keys_found == 3){
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&mutex);

    

    //printf("keys found currently: %d\n", keys_found);
    int* array = (int*) args;
    int i, j, n, found_key = -1;
    n = array[0]; //can't guarantee that all array sizes will be same  (if our num workers doesnt divide)
    j = array[1]; //starting index will help us keep track where the hidden keys are
    

    
    //printf("size of mini array: %d\n", n);
    for (i = 2; i < n+2; i++, j++){ //n+2 because we need to account for the first 2 extra elements
        if (keys_found == 3) break;
        if (array[i] == -50){ //hidden key found
            //printf("hidden key found at %d!\n", j);
            found_key = j;
            keys_found++;
        }
    }

        


    int* ret = (int*) found_key;
    pthread_mutex_unlock(&mutex);
    //printf("found key result = %d\n", found_key);
    free(array);
    return (void*) ret;
}

int main(int argc, char* argv[]){ 
    //if argc, that's how many threads 
    struct timeval start,end;
<<<<<<< HEAD
    int size = 1000, piece_size =  200; //TODO THIS WILL CHANGE
=======
    int size = 1000, piece_size = 10; //TODO THIS WILL CHANGE
>>>>>>> parent of bff5b04... hopefully finished

    int numWorkers, i,j,k,n;
    FILE* fp = fopen("1k_items.txt",  "r"); //TODO THIS WILL CHANGE
    if (fp == NULL) return -1;

    if (argc > 1){
        numWorkers = atoi(argv[1]);
        if (numWorkers > size) numWorkers = size;
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
        
    }
    //join 
    int found = 0;
    for (i = 0; i < numWorkers; i++){
        
        //join and get return val     
        int ret; 
        
        pthread_join(threads[i], (void*)&ret);
        
        if(ret != NULL && ret!= -1){
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", &threads[i], ret); 
            found++;
            
        }
        if(found == 3){
            printf("we found em all. should be done \n");
            break;
        }
        // else {
        //     printf("here\n");
        //     continue;
        // }
        
    }

    fclose(fp);

    //end timing
    gettimeofday(&end,NULL);
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
<<<<<<< HEAD
    //printf("Time of execution to check %s items with %d threads: %f usec\n", "1k", numWorkers, runTime); //TODO this will change
=======
    //printf("time of execution: %f usec\n", runTime);
>>>>>>> parent of bff5b04... hopefully finished

    


}


