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
void processTree(){

  int i;
  pid_t child;
  pid_t gchild;
  int val = 2;
  int child_num;
  child = fork();
  gchild = fork();

  tree_node* ptr = (tree_node*)malloc(sizeof(tree_node)); 


    if(child<0 && gchild <0){
    //error

    }else if(child>0 && gchild>0){
    //parent -- wait for child
     ptr = createTree(_root,"Parent: A"); 
     int status;
      printf("Parent waiting for child to terminate\n");
      waitpid(child, &status, 0);
      child_num = WEXITSTATUS(status);	      
	printf("Child %d terminated\n",child_num+1);
    }else if(child == 0 && gchild>0){
      //first child
	ptr = createTree(_root,"Child: B");
	int sstatus;
	printf("Child waiting for child to terminate\n");
	waitpid(gchild,&sstatus,0);
	printf("Grandchild terminated\n");
	printf("Child Process: B, returns: 4, pid: %d ppid: %d\n", getpid(),getppid());
	exit(4); 
    }else if(child>0 && gchild == 0){
	//second child
	ptr = createTree(_root, "Child: C");
 	printf("Child Proces: C, returns: 6, pid: %d, ppid: %d\n", getpid(), getppid());
	exit(6);
    }else{
	//grandchild
	ptr = createTree(_root->children[0], "Child: D");
	printf("Child Process: D, returns: 10, pid: %d, ppid: %d\n", getpid(), getppid());
	exit(10);
    }
    printf("Parent Process: A, returns: %d, pid: %d, ppid: %d\n", val, getpid(), getppid());
    return;
}

int main(){

processTree();
print_tree(_root);
}

