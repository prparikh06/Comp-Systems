#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>


int* array;
int size;
int piece_size;
pthread_t* threads;



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
    - local max
    - index of local max
    - index of key (if found)
*/
    
void* thread_search(void* args){

    int* index = (int*) args;
    int i, max, max_index, found_key = -1;

    int start = index[0]; //starting index
    int end = index[1]; //ending index 

    if (end > size) end = size; //dont want index to go beyond size    
    max = 0;
    //ret vals
	int* return_vals = (int*) malloc(sizeof(int) * 5);
	return_vals[0] = -1; return_vals[1] = -1; return_vals[2] = -1; return_vals[3] = -1; return_vals[4] = -1;
    int key_index = 2; //in case a thread finds more than 1 hidden key, keep a key_index for the return_vals array (increment each time its found) 
    for (i = start; i < end; i++){
		
        if (array[i] == -50){ //hidden key found
				return_vals[key_index++] = i;
        }
        if (array[i] > max){
            max = array[i];
            max_index = i;
        }
        
    }


    return_vals[0] = max;
    return_vals[1] = max_index;
	free(index);
    return (void*) return_vals;
} 


int main(int argc, char* argv[]) { 
     
    struct timeval start,end; 
    int numWorkers, i,k,n;
	
	FILE* foutp = fopen("output.txt", "w+");

    char* file = "1m_items.txt"; //TODO this will change
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
	     piece_size = ceil(((double)size/numWorkers)); 
    }
    else{

        //divide by pieces size
        numWorkers = size/piece_size; 
    }



    threads = (pthread_t*) malloc(sizeof(pthread_t)*numWorkers);

    int finalMax = -1, finalMax_index = -1, max_threadID = 0;

    //create array
    array = malloc(sizeof(int) * size);
    
    for (i = 0; i < size; i++){
        if (fscanf(fp,"%d\n", &n) == EOF) break;
        array[i] = n;
    }
	 fclose(fp);
    //start timing
    gettimeofday(&start,NULL);
    
    for (i = 0, k = 0; k < numWorkers; i+=piece_size, k++){

        pthread_t curr_thread;
	if (i > size) i = size; 
        int* args = malloc(sizeof(int)*2);
        args[0] = i; args[1] = i+piece_size;
	pthread_create(&curr_thread, NULL, thread_search, (void*) args);

	printf("Created thread %u\n", curr_thread);     
	fprintf(foutp, "Created thread %u\n", curr_thread);	
	threads[k] = curr_thread;

    }
	
	
	
    //join and get absolute max
    for (i = 0; i < numWorkers; i++){
        //join and get return vals        
        int* return_vals;// = (int*) malloc(sizeof(int) * 5); //return vals are local max, max index, indices of the keys (if found)

        pthread_join(threads[i], &return_vals);
        //printf("return vals are = %d, %d, %d for thread %u\n", return_vals[0], return_vals[1], return_vals[2], &threads[i]);

        int currMax = (int) return_vals[0];

        if (return_vals[2] != -1) //a key has been found
		{
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[2]);
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[2]);
        }
        if (return_vals[3] != -1)
		{			//another has been found
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[3]);
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[3]);
		}
        if (return_vals[4] != -1) //another has been found
		{
            printf("Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[4]);
			fprintf(foutp, "Hi I am Pthread %u and I found the hidden key in position A[%d]\n", threads[i], (int) return_vals[4]);
		}
        if (currMax > finalMax){
            finalMax = currMax;
            finalMax_index =  (int) return_vals[1];          
            max_threadID = threads[i];
        }
	free(return_vals);
    }
	//end timing
    gettimeofday(&end,NULL);

	free(array);
    
    float runTime = (float) end.tv_usec - start.tv_usec + 1000000*(end.tv_sec - start.tv_sec);
    printf("Hi I am Pthread %u and I found the maximum value %d in position A[%d]\n", max_threadID, finalMax, finalMax_index);
	fprintf(foutp, "Hi I am Pthread %u and I found the maximum value %d in position A[%d]\n", max_threadID, finalMax, finalMax_index);

    //printf("Time of execution to check %d items with %d threads: %f usec\n", size, numWorkers, runTime); 

	fclose(foutp);
    

}
