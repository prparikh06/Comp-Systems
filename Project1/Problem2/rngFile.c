// C program to generate random numbers 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
  

// Assume user formats input correctly
int lazyNumItems(char* s)
{
	int multiplier = 1;
	char* c;
	int length = 0;
	for (c = &s[0]; *c != '\0'; c++)
	{
		if (strncmp(c, "k", 1)==0)
		{
			multiplier = 1000;
		}
		else if (strncmp(c, "m", 1)==0)
		{
			multiplier = 1000000;
		}
		length++;
	}
	char substr[length - 1];
	memcpy(substr, &s[0], length-1);
	substr[length-1] = '\0';
	
	return atoi(substr)*multiplier;
}

  
// Driver program 
int main(int argc, char *argv[]) 
{ 
	// This program will create same sequence of  
	// random numbers on every program run  
	// going to run it for 1 mill, 100k, 10k, 1k random numbers and output them to textfiles
	
    FILE *fp;
	
	int numItems = lazyNumItems(argv[1]);//atoi(argv[1]);
		
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
		if (i == numItems/4 || i == numItems/2 || i == numItems*3/4)
		{
			temp = -50;
		}
        fprintf(fp,"%d\n", temp);
	}

	fclose(fp);
 
	return 0; 
} 

