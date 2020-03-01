// C program to generate random numbers 
#include <stdio.h> 
#include <stdlib.h> 
  
// Driver program 
int main(int argc, char *argv[]) 
{ 
	// This program will create same sequence of  
	// random numbers on every program run  
	// going to run it for 1 mill, 100k, 10k, 1k random numbers and output them to textfiles
	
     	FILE *fp;

	int numItems = atoi(argv[1]);
		
	char buf[100];
	snprintf(buf,sizeof(buf),"%s_items.txt",argv[1]);
	fp = fopen(buf,"w");
	
	if (fp == NULL) {
		perror("txtfile not made\n");
		exit(1);
	}


	int i;
	for(i = 0; i<numItems; i++){ 
		int temp = rand();
        	fprintf(fp,"%d\n", temp);
	}

	fclose(fp);
 
	return 0; 
} 

