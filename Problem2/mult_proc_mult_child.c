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
	int buf[BUFF_SIZE];			//4 megabytes (×_×#)
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

int getMax(int curDepth, int depth, int nchild, int *uniqueIDs, int dataSectionSize, int shmid, char *outputPath, int secretElement)
{
	// Make a pipe for each child
	int p[2*nchild];
	int a;
	for (a = 0; a < nchild; a++)
	{
		if (pipe(&p[2*a]) < 0)
			exit(1);
	}
	
	if (curDepth == depth)
	{
		
		int dataStartIndex = uniqueIDs[0]*dataSectionSize;
		struct shmseg *shmp = shmat(shmid, NULL, 0);
		
		// find max for my section in data
		int max = -1;
		int end = (shmp->actual_length < dataStartIndex+dataSectionSize) ? shmp->actual_length : dataStartIndex+dataSectionSize;
		
		int i;
		for (i = dataStartIndex; i < end; i++)
		{
			max = (int) (shmp->buf[i] > max) ? shmp->buf[i] : max;
			
			if (shmp->buf[i] == secretElement)
			{
				FILE *outpt = fopen(strcat(outputPath,".tmp"), "a");
				fprintf(outpt, "Hi I'm process %d and I found the hidden key in position %d. \n", getpid(), i);
				fclose(outpt);
			}
		}
	
		shmdt(shmp);
		return max;
	}
	
	int i;
	int max = -1;
	for (i = 0; i < nchild; i++)
	{
		int *subarr = getUniqueSubArrForChild(uniqueIDs, i, (int)pow(nchild, (depth-(curDepth+1))));
		int pid = fork();
		if (pid == 0)
		{
			FILE *outp = fopen(outputPath, "a");
			fprintf(outp, "Hi I'm process %d and my parent is %d. \n", getpid(), getppid());
			fclose(outp);
			
			int max = getMax(++curDepth, depth, nchild, subarr, dataSectionSize, shmid, outputPath, secretElement);
			
			close(p[2*i]);
			write(p[2*i + 1], &max, sizeof(int)); // pipe up max
			close(p[2*i + 1]);

			
			free(subarr);
			exit(0);
		}
		else if (pid > 0)
		{
			int recvPID = wait(NULL);
			int recv;
			close(p[2*i + 1]);
			read(p[2*i], &recv, sizeof(int));		// recv max from child
			close(p[2*i]);
			max = (recv>max)?recv:max;
		}
	}
	return max;
}


int main(int argvc, char *argv[])
{
	int depth = atoi(argv[1]);
	int nchild = atoi(argv[2]);
	char* fileName = argv[3];
	char* outputPath = argv[4];
	
	FILE *fp = fopen(fileName,"r");

	if(fp == NULL){
		perror("Error: File not opened :( \n");
		exit(-1);	
	}
	
	/** Create Shared Memory **/
	struct shmseg *shmp;
	int shmid = shmget(IPC_PRIVATE, sizeof(struct shmseg), IPC_CREAT | 0777);		// Memory segment ID
	shmp = shmat(shmid, NULL, 0);		// Shared memory address, Attach shared memory segment to address space
	/**************************/
	
	/** Copy data file into shared memory **/
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

	int numLeafProc = (int) pow(nchild, depth);
	int dataSizePerProc = (shmp->actual_length) / numLeafProc;
	
	shmdt(shmp);
	
	int *uniqueIDs = makeIndexArray(numLeafProc);
	int max = getMax(0, depth, nchild, uniqueIDs, dataSizePerProc, shmid, outputPath, -50);
	free(uniqueIDs);
	
	FILE *outp = fopen(outputPath, "a");
	fprintf(outp, "Max=%d\n", max);
	
	char *tempPath = strcat(outputPath,".tmp");
	// Formating our output correctly
	FILE *outpt = fopen(tempPath, "r");
	char c;
	fprintf(outp, "\n");
	while (c=fgetc(outpt))
	{
		if (c == EOF) break;
		fprintf(outp, "%c", c);
	}

	fclose(outpt);
	fclose(outp);
	
	remove(tempPath);
	
	return 0;
}