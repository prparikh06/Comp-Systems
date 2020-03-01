#include <stdio.h>
#include <stdlib.h>


/*
Program reads in a txt file and performs a linear search (1 process) to determine the MAX and outputs the max value
*/

int main(int argc, char *argv[]) {

	char* fileName = argv[1];
	
	FILE *fp = fopen(fileName,"r");

	if(fp == NULL){
		perror("Error: File not opened :( \n");
		exit(-1);	
	}


	int max = 0;
	int num;
	while( fscanf(fp,"%d \n", &num) == 1) {
		
		if(num > max)
			max = num;
	}
	
	printf("MAX: %d \n",max);

	return max;
}
