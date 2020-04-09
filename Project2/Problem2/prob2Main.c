#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "procFunctions.c"

int main(int argc, char* argv[]){

	FILE *fp;
	char *filename;
	tree_node *ptr = (tree_node*)malloc(sizeof(tree_node));

	if(argc < 2){

  	  printf("Missing Filename\n");
	  return 1;
	}
	filename = argv[1];
	fp = fopen(filename, "r");

	if(fp){
	  	ptr = read_tree_file(filename);
		generate(ptr);
		print_tree(ptr);

	}else{
	  printf("Failed to open the file\n");
	}

	return 0;
}
