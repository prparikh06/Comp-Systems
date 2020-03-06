#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>


#define NCHILD 5
#define BUFF_SIZE 1000000			//4 megabytes (×_×#)


struct shmseg
{
	int actual_length;
	int buf[BUFF_SIZE];
};

int main(int argvc, char *argv[])
{
	char* fileName = argv[1];
	
	FILE *fp = fopen(fileName,"r");

	if(fp == NULL){
		perror("Error: File not opened :( \n");
		exit(-1);	
	}
	
	/** Create Shared Memory **/
	struct shmseg *shmp;
	//shmp->buf = malloc(BUFF_SIZE*sizeof(int));
	int shmid = shmget(IPC_PRIVATE, sizeof(struct shmseg), IPC_CREAT | 0777);		// Memory segment ID
	shmp = shmat(shmid, NULL, 0);		// Shared memory address, Attach shared memory segment to address space
	/**************************/
	
	int num;
	int *bufptr = shmp->buf;
	int count = 0;
	while(fscanf(fp,"%d \n", &num) == 1) {
		memcpy(bufptr, &num, sizeof(int));
		count++;
		shmp->actual_length = count;
		bufptr++;
	}
	
	shmdt(shmp);		// Detach from memory segment
	
	int pid = fork();
	if (pid == 0)
	{
		shmp = shmat(shmid, NULL, 0);
		int i;
		for (i = 0; i < shmp->actual_length; i++)
		{
			printf("%d\n", shmp->buf[i]);
		}
		
		shmdt(shmp);		// Detach from memory segment
	}
	else if (pid > 0)
	{
		wait();
	}
	
	return 0;
}