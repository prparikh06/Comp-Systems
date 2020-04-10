#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

pthread_t thread;
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int keys_found = 0; 

void* thread_serach(void* args){  

    int* array = (int*) args;
    int i, n;
    n = array[0]; //can't guarantee that all array sizes will be same  (if our num workers doesnt divide)
    int max = -1, max_index = -1;
    int* ret = malloc(sizeof(int)*5);
    for (i = 1; i < n+1, keys_found < 3; i++){ 
        if (array[i] > max){
            max = array[i-1];
            max_index = i-1;
        }
        if (array[i] == -50){ //hidden key found
            ret[2+keys_found++] = i-1;
        }
    }
    ret[0] = max;
    ret[1] = max_index;
    free(array);
    return (void*) ret;
}

int main(int argc, char* argv[]){ 
    //if argc, that's how many threads 
    struct timeval start,end;
    int size = 1000; //TODO THIS WILL CHANGE
    int numWorkers, i,n;
    FILE* fp = fopen("1k_items.txt",  "r"); //TODO THIS WILL CHANGE
    if (fp == NULL) return -1;

    //start timing
    gettimeofday(&start,NULL);
    int* array = malloc(sizeof(int) * (size + 1));
    array[0] = size;
    for (i = 1; i < size; i++){
        if (fscanf(fp,"%d\n", &n) == EOF) break;
        //printf("adding item %d to mini array\n", n);
        array[i] = n;
    }

    //create     
    pthread_create(&thread, NULL, thread_serach, (void*) array);
    //join and get return vals     
    int* ret = malloc(sizeof(int)*5); 
    pthread_join(thread, (void*) &ret);
    printf("Hi I am Pthread %u and I found the hidden keys at A[%d] A[%d] and A[%d]\n", &thread, ret[2], ret[3], ret[4]);
    printf("I found max value %d at A[%d]\n", ret[0], ret[1]);
    fclose(fp);

    //end timing
    gettimeofday(&end,NULL);
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
    //printf("Time of execution for one thread to check 1M items: %f usec\n", runTime);

    


}


