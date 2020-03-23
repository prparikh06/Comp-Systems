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



int getMax(int procNum, int nChild, int dataSectionSize, int shmid){

	//pip for each child
	int p[2];
	if(pipe(&p[2]) < 0)
		exit(1);

	int max=-1;	
	int pid=fork();
	
	
	
	if(pid == 0) { //child
		procNum = procNum+1;
		printf("Hi I'm process %d and my parent is %d. \n",getpid(),getppid());
		
		if(procNum<=nChild){
			 max = getMax(procNum,nChild,dataSectionSize,shmid);
		}
		struct shmseg *shmp = shmat(shmid,NULL,0);
		int i;
		for(i=procNum*dataSectionSize;i<procNum*dataSectionSize+dataSectionSize;i++){
			
			max = (int) (shmp->buf[i] > max)? shmp->buf[i]:max;
		}
		shmdt(shmp);
		int recvPID = wait(NULL);
		int recv;
		read(p[0],&recv,sizeof(int));
		max = (p[0]>max)?p[0]:max;
		close(p[0]);
                write(p[1], &max, sizeof(int)); // pipe up max
                close(p[1]);
		exit(0);
	}

		else if (pid > 0) {
			struct shmseg *shmp = shmat(shmid,NULL,0);
			int i;
			for(i =0;i<dataSectionSize;i++){
				max = (int) (shmp->buf[i] > max) ? shmp->buf[i] : max;
			}
			shmdt(shmp);
			int recvPID = wait(NULL);
			int recv;
			close(p[1]);
			read(p[0],&recv,sizeof(int));
			close(p[0]);
			
			max = (recv>max)?recv:max;
		
		}
	return max;
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

        int dataSizePerProc = (shmp->actual_length) / nChild;

        shmdt(shmp);

        int max = getMax(0, nChild, dataSizePerProc, shmid);
        printf("Final max is:  %d.\n", max);


        return 0;
}	
