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
tree_node* _root = NULL;
void print_tree(tree_node *root){
  _print_tree(root,0);
}


tree_node* createTree(char* proc){
	int x = 0;
	tree_node* ptr = (tree_node*)malloc(sizeof(tree_node));
	tree_node* cptr = (tree_node*)malloc(sizeof(tree_node));
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

