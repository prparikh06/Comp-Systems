#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t threads[];
int global_index = 0;

void* thread_serach(void* args){
    printf("thread %d is searching starting at global index %d\n", pthread_self(), global_index);
    int* array = (int*) args;
    int i, n, max;
    n = sizeof(array)/sizeof(int); //can't guarantee that all array sizes will be same right (if our num workers doesnt divide)
    max = array[0];
    global_index++;
    for (i = 1; i < n; i++){
        if (array[i] > max)
            max = array[i];
        if (array[i] == -50){ //hidden key found
            printf("hidden key found at %d!\n", global_index);
        }
        global_index++;

    }
    int* return_vals = (int*) malloc(sizeof(int) * 2);
    


}

int main(int argc, char* argv[]){ 
    //if argc, that's how many threads 
    int numWorkers, i,j,n;
    FILE fp* = fopen("test.txt",  "r"); //TODO this will change
    if (fp == NULL) return -1;

    if (argc > 0){
        numWorkers = atoi(argv[1]);
        threads[numWorkers];
        for (i = 0; i < numWorkers; i++){
            //create threads 
            mini_array_size = ceil(20.00/numWorkers); //TODO this will change

        }
    }

    else{
        //call recursively for pieces of 10
        numWorkers = 20/10; //TODO this will change
        threads[numWorkers];
        mini_array_size = 10;
    }
    for (i = 0; i < numWorkers; i++){ 
        pthread_t curr_thread;
        //create mini array
        int* mini_array = (int*) malloc(sizeof(int) * mini_array_size);
        for (j = 0; j < mini_array_size; j++){
            if (fscanf("%d\n", &n) == EOF) break;
            mini_array[j] = n;
        }
        //mini array done, time to thread
        pthread_create(&curr_thread, NULL, thread_serach, (void*) mini_array);
        threads[i] = curr_thread;
    }

    int finalMaximums[numWorkers];
    //join all threads
    for (i = 0; i < numWorkers; i++){
        int* return_vals = (int*) malloc(sizeof(int) * 2);
        //RETURN VALUES: [max, index of hidden key (if not found, do -1)]
        pthread_join(threads[i], (void*) &return_vals);
        printf("thread %d has return values of %d, and %d\n", threads[i],(int) return_vals[0], (int) return_vals[1]);
    }

}


