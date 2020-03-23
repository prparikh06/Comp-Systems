#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tree_node{
  unsigned nr_children;
  char* name;
  struct tree_node** children;
}typedef tree_node;
static void _print_tree(tree_node *root, int level){
  int i;
  for(i = 0; i<level; i++)
    printf("\t");
  printf("%s\n", root->name);
  
  for(i = 0; i < root->nr_children; i++){
    _print_tree(root->children[i], level + 1);
  }
}
tree_node* _root = NULL;
void print_tree(tree_node *root){
  _print_tree(root,0);
}
tree_node* createTree(tree_node* root, char* name){
  tree_node* ptr = (tree_node*)malloc(sizeof(tree_node));
    ptr->name = malloc(strlen(name)); 
    ptr->children = (tree_node**)malloc(sizeof(tree_node*));
  if(_root == NULL){//init root of tree
    ptr->name = name;
    ptr->nr_children = 0;
    ptr->children = NULL;
    _root = ptr;
  }else if(root == NULL){
    ptr->name = name;
    ptr->nr_children = 0;
    ptr->children = NULL;
  }else{//node is a child
    if(!(root->nr_children)){//inserting first child
      root->children = (tree_node**)malloc(sizeof(tree_node*));
      root->children[root->nr_children++] = createTree(NULL, name);
      ptr = root->children[root->nr_children - 1];
    }else{
      root->children[root->nr_children++] = createTree(NULL, name);
      ptr = root->children[root->nr_children - 1];
    }
  }
  return ptr;
}
