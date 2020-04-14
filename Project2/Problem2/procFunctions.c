#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tree_node{
  int nr_children;
  char name;
  struct tree_node** children;
}typedef tree_node;

static void _print_tree(tree_node *root, int level){
  int i;
  for(i = 0; i<level; i++)
    printf("\t");
  printf("%c\n", root->name);
  
  for(i = 0; i < root->nr_children; i++){
    _print_tree(root->children[i], level + 1);
  }
}

void print_tree(tree_node *root){
  _print_tree(root,0);
}

void generate(tree_node* ptr){//generates the processes
	int x = 0;
	pid_t child;
	printf("Starting Process: %c\n", ptr->name);
	if(ptr->nr_children == 0){
		sleep(10);
	}else{
	  while(x < ptr->nr_children){
	    child = fork();
	    if(child > 0){
		int status;
		waitpid(child,&status,0);
	    }else{
		printf("Forking Processs: %c\n", ptr->children[x]->name);
		_exit(1);
	    }
	    generate(ptr->children[x]);
	    x++;
	  }
	}
	printf("Terminating process: %c\n", ptr->name);
}

tree_node* createTree(char* proc){//creates the tree struct
	int x = 0;
	tree_node* ptr = (tree_node*)malloc(sizeof(tree_node));
	ptr->name = proc[0];
	ptr->nr_children = proc[2] - '0';
	ptr->children = NULL;
	if(ptr->nr_children > 0){
	  ptr->children = (tree_node**)malloc(sizeof(tree_node*)*ptr->nr_children);
	  while(x < ptr->nr_children){
	    ptr->children[x] = (tree_node*)malloc(sizeof(tree_node));
	    ptr->children[x]->name = proc[4 + 2*x];	
	    x++;
	  }
	}
	return ptr;
}


tree_node* read_tree_file(const char *filename){
	FILE *fp = fopen(filename, "r");
	char proc[256];
	char root = '\0';
	char name;
	tree_node *ptr = (tree_node*)malloc(sizeof(tree_node));
	int i = 0;
	int j = 0;
	while(fgets(proc,sizeof(proc),fp)){
		
		if(root == '\0'){
		  root = proc[0];
		  ptr = createTree(proc);
		}
		else{
		  int x = 0;
		  while(ptr->children[x] != NULL){
		    name = ptr->children[x]->name;
		    if(name ==proc[0]){
			ptr->children[x] = createTree(proc);
			break;
		    }
		    x++;
		  }
	       }
	  }

	return ptr;
}
