#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int mini_array_size;
pthread_t* threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int keys_found = 0; 



/*
    data to be returned:
    - index of key (if found)
*/

void* thread_serach(void* args){

   if (keys_found == 3){
        pthread_exit(NULL);
    }

    pthread_mutex_lock(&mutex);

    int* array = (int*) args;
    int i, j, n, found_key = -1;
    n = array[0]; //can't guarantee that all array sizes will be same  (if our num workers doesnt divide)
    j = array[1]; //starting index will help us keep track where the hidden keys are
    

    for (i = 2; i < n+2; i++, j++){ //n+2 because we need to account for the first 2 extra elements
        if (keys_found == 3) break;
        if (array[i] == -50){ //hidden key found
            found_key = j;
            keys_found++;
        }
    }
    
    int* ret = (int*) found_key;
    pthread_mutex_unlock(&mutex);
    free(array);
    return (void*) ret;
}

/*
    DEFAULT PIECE SIZE WILL VARY DEPENDING ON SIZE OF INPUT/TEXT FILE
    piece_size = 200 if 1m
    piece_size = 100 if 100k
    piece_size = 50 if 10k
    piece_size = 10 if 1k

*/

int main(int argc, char* argv[]){ 
    //if argc, that's how many threads 
    struct timeval start,end;
    int size = 1000000, piece_size =  200; //TODO THIS WILL CHANGE

    int numWorkers, i,j,k,n;
    FILE* fp = fopen("1m_items.txt",  "r"); //TODO THIS WILL CHANGE
    if (fp == NULL) {
		printf("Could not open file.\n");   
    	return -1;
	}
    if (argc > 1){
        numWorkers = atoi(argv[1]);
        for (i = 0; i < numWorkers; i++){
            //create threads 
            mini_array_size = ceil((double)size/numWorkers); 
        }
    }
    else{
        //divide by pieces size
        numWorkers = size/piece_size; 
        mini_array_size = piece_size;
    }
    
    threads = (pthread_t*) malloc(sizeof(pthread_t)*numWorkers);



    for (i = 0, k = 0; k < numWorkers; i+=j-2, k++){  //this is a little OD but it's fine
        pthread_t curr_thread;        

        //create mini array
        int* mini_array = (int*) malloc(sizeof(int) * (mini_array_size + 2)); //adding 2 in order to add size and starting index
        for (j = 2; j < mini_array_size+2; j++){
            if (fscanf(fp,"%d\n", &n) == EOF) break;
            mini_array[j] = n;
        }
        
        mini_array[0] = j-2; //size of array
        mini_array[1] = i; //starting index
        
        //mini array done, time to thread
        //start timing
        gettimeofday(&start,NULL);
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

            break;
        }
       
    }

    fclose(fp);

    //end timing
    gettimeofday(&end,NULL);
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
    //printf("Time of execution to check %s items with %d threads: %f usec\n", "1k", numWorkers, runTime); //TODO this will change

    

}


