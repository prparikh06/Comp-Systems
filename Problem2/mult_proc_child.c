#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <math.h>

#define BUFF_SIZE 1000000


struct shmseg
{
        int actual_length;
        int buf[BUFF_SIZE];                     //4 megabytes (×_×#)
};

int *makeIndexArray(int n)
{
        int *ret = malloc(sizeof(int)*n);
        int i;
        for (i = 0; i < n; i++)
        {
                ret[i] = i;
        }
        return ret;
}
int *getUniqueSubArrForChild(int arr[], int curChild, int size)
{
        int *ret = malloc(sizeof(int)*size);
        int start = curChild*size;
        int i;
        for (i = start; i < start+size; i++)
        {
                ret[i-start] = arr[i];
        }
        return ret;
}


int getMax(int nChild, int *uniqueIDs, int dataSectionSize, int shmid){

	//pip for each child
	int p[2*nchild];
	int a;
	
	for(a=0;a<nchild;a++){
		if(pipe(&ap[2*a]) < 0)
			exit(1);
	}

	int i;
	int max=-1;
	
	for(i=0;i<nchild;i++){

		int *subarr = getUniqueSubArrForChild(uniqueIDs,i,2);
		int pid=fork();

		if(pid == 0) { //child
		
			printf("Hi I'm process %d and my parent is %d. \n",getpid(),getppid());
			int max = getMax(nchild,subarr,dataSectionSize,shmid);
			close(p[2*i]);
                        write(p[2*i + 1], &max, sizeof(int)); // pipe up max
                        close(p[2*i + 1]);
                        exit(0);

		}

		else if (pid > 0) {
			int recvPID = wait(NULL);
			int recv;
		

	return 0;
}


int main (int argc, char *argv[]){

	int nChild = atoi(argv[1]);
	char *filename = argv[2];


	FILE *fp = fopen(filename,"r");

	if(fp == NULL){
		perror("ERROR: file not opened :( \n");
		exit(-1);
	}


	// create shared memory
	struct shmseg *shmp;
	int shmid = shmget(IPC_PRIVATE,sizeof(struct shmseg),IPC_CREAT | 0777); //mem segment ID
	shmp = shmat(shmid,NULL,0); //shared memory address pointer


	//copy file data into shared memory
	/****************************************/
	int num;
        int *bufptr = shmp->buf;
        int count = 0;
        while(fscanf(fp,"%d \n", &num) == 1) {
                memcpy(bufptr, &num, sizeof(int));
                count++;
                shmp->actual_length = count;
                bufptr++;
        }
        /***************************************/

        int numLeafProc = (int) nchild;
        int dataSizePerProc = (shmp->actual_length) / numLeafProc;

        shmdt(shmp);

        int *uniqueIDs = makeIndexArray(numLeafProc);
        int max = getMax(0, nchild, uniqueIDs, dataSizePerProc, shmid);
        printf("Final max is:  %d.\n", max);


        return 0;
}	
