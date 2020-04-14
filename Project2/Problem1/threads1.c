#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

int* array;
int size;
int piece_size;
pthread_t* threads;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int keys_found = 0; 


int get_size(char* file){

	FILE* fp = fopen(file,"r");
	int n, count = 0;
	while (fscanf(fp, "%d",&n) != EOF) {
		count++;
	}
  	fclose(fp);
	return count;
}


/*
    data to be returned:
    - index of key (if found)
*/

void* thread_search(void* args){

   if (keys_found == 3){
		  //printf("exiting...\n");
		free(args);
        pthread_exit(NULL); 
    }

    //lock protect keys_found
    pthread_mutex_lock(&mutex);

    int* index = (int*) args;
    int i, found_key = -1;
    int start = index[0]; //starting index
    int end = index[1]; //ending index 
	if (end > size) end = size; //dont want index to go beyond size
    
    //ret vals
	int* return_vals = (int*) malloc(sizeof(int) * 3);
	return_vals[0] = -1; return_vals[1] = -1; return_vals[2] = -1;
    int key_index = 0; //in case a thread finds more than 1 hidden key, keep a key_index for the return_vals array (increment each time its found) 
    
    for (i = start; i < end; i ++){
        if (array[i] == -50){ //hidden key found
            return_vals[key_index++] = i;
            keys_found++;
        }
        if (keys_found == 3) break;

    }

	free(args);
    pthread_mutex_unlock(&mutex);
    return (void*) return_vals;
}



int main(int argc, char* argv[]){ 
    struct timeval start,end; 
    int i,k,n,numWorkers;
    char* file = "1m_items.txt"; //TODO this will change
	
	FILE* foutp = fopen("output.txt", "w+");
    
    FILE* fp = fopen(file, "r"); 
    if (fp == NULL) {
		printf("Could not open file.\n");   
    	return -1;
	}  
    size = get_size(file);

       
/*
    DEFAULT PIECE SIZE WILL VARY DEPENDING ON SIZE OF INPUT/TEXT FILE
    piece_size = 200 if 1m
    piece_size = 100 if 100k
    piece_size = 50 if 10k
    piece_size = 10 if 1k

*/
    if (size == 1000000) piece_size = 10000;
    else if (size == 100000) piece_size = 1000;
    else if (size == 10000) piece_size = 100;
    else piece_size = 10;

    
    if (argc > 1){ 

        numWorkers = atoi(argv[1]);
        piece_size = ceil((double)size/numWorkers); 
        
    }
    else{
        //divide by pieces size
        numWorkers = size/piece_size; 
        
    }

    threads = (pthread_t*) malloc(sizeof(pthread_t)*numWorkers);

    //create array
    array = malloc(sizeof(int) * size);
    for (i = 0; i < size; i++){
        if (fscanf(fp,"%d\n", &n) == EOF) break;
        array[i] = n;
    }

    //start timing
    gettimeofday(&start,NULL);
    
    for (i = 0, k = 0; k < numWorkers; i+=piece_size, k++){
        pthread_t curr_thread;
        int* args = malloc(sizeof(int)*2);
        args[0] = i; args[1] = i+piece_size;
        pthread_create(&curr_thread, NULL, thread_search, (void*) args);
		printf("Created thread %u\n", curr_thread);
		fprintf(foutp, "Created thread %u\n", curr_thread);
        threads[k] = curr_thread;
    }

    //join 
    int found = 0;
    for (i = 0; i < numWorkers; i++){
        
        //join and get return val     
        int* ret; //ret vals for the indices of the hidden key (if any) 
        pthread_join(threads[i], &ret);
        
		if (ret == NULL)
		{
			continue;
		}
		
        if(ret[0] != -1){ //hidden key found
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[0]);
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[0]);			
            found++;
        }
        if(ret[1] != -1){ //another key found
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[1]); 
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[1]);
            found++;   
        }
        
        if(ret[2] != -1){
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[2]); 
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], ret[2]);
            found++;
            
        }

        if(found == 3){
        	//printf("all three found\n");
			free(ret);
            break;
        }
       
	   free(ret);
    }

	free(array);
	free(threads);
	
    fclose(fp);

    //end timing
    gettimeofday(&end,NULL);
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
    //printf("Time of execution to check %d items with %d threads: %f usec\n", size, numWorkers, runTime); 

    fclose(foutp);

}


