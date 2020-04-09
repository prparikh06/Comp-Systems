#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "procTree.c"


void generate(tree_node* ptr){
	int x = 0;
	if(ptr->nr_children == 0){
		sleep(10);
	}else{
	  while(x < ptr->nr_children){
	    fork();
	    generate(ptr->children[x]);
	    x++;
	  }
	printf("Hello this is process: %d\n", getpid());
	}



}




int main(int argc, char* argv[]){


	FILE *fp;
	char *filename;
	char proc[256];
	char root = '\0';
	tree_node *ptr = (tree_node*)malloc(sizeof(tree_node));

	if(argc < 2){

  	  printf("Missing Filename\n");
	  return 1;
	}
	filename = argv[1];
	fp = fopen(filename, "r");

	if(fp){
	  while(fgets(proc,sizeof(proc),fp)){//loop through each line
		
		if(root == '\0'){
		  root = proc[0];
		  ptr = createTree(proc);
		}
		else{
		  int x = 0;
		  while(ptr->children[x] != NULL){
		    char name = ptr->children[x]->name;
		    if(name ==proc[0]){
			ptr->children[x] = createTree(proc);
			break;
		    }
		    x++;
		  }
	 	}
	  }
		//generate(ptr);
		print_tree(ptr);

	}else{
	  printf("Failed to open the file\n");
	}

	return 0;
}
