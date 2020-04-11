#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t threads[];

void* thread_serach2(void* args){

}

void* thread_search1(void* args){

}

int main(int argc, char* argv[]){ 
    //if argc, that's how many threads 
    int numWorkers, i;
    FILE fp* = fopen("1k_test.txt",  "r");
    if (fp == NULL) return -1;

    if (argc > 0){
        numWorkers = atoi(argv[1]);
        threads[numWorkers];
        for (i = 0; i < numWorkers; i++){
            //create threads 
            mini_array_size = ceil(1000.00/numWorkers); //TODO this will change

        }
    }

    else{
        //call recursively for pieces of 10
        numWorkers = 1000/10; //TODO this will change
        threads[numWorkers];
        mini_array_size = 10;
        while(fscanf("%d\n") != EOF){

        }

    }


}