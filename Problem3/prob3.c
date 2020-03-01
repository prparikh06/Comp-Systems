#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

struct tree_node{
  unsigned nr_children;
  char name[8];
  struct tree_node *children;
}typedef tree_node;

static void _print_tree(tree_node *root, int level){
  int i;
  for(i = 0; i<level; i++)
    printf("\t");
  printf("%s\n", root->name);
  
  for(i = 0; i < root->nr_children; i++){
    _print_tree(root->children + i, level + 1);
  }
}

void print_tree(tree_node *root){
  _print_tree(root,0);
}

void processTree(){

  int i;
  pid_t child;
  pid_t gchild;
  int val = 0;
  int child_num;
  child = fork();
  gchild = fork();
    if(child<0 && gchild <0){
    //error

    }else if(child>0 && gchild>0){
    //parent -- wait for child
      int status;
      printf("Parent waiting for child to terminate\n");
      waitpid(child, &status, 0);
      child_num = WEXITSTATUS(status);	      
	printf("Child %d terminated\n",child_num+1);
    }else if(child == 0 && gchild>0){
      //first child
	int sstatus;
	printf("Child waiting for child to terminate\n");
	waitpid(gchild,&sstatus,0);
	printf("Grandchild terminated\n");
	printf("Child Process: B, returns: %d, pid: %d ppid: %d\n",val, getpid(),getppid());
	exit(val); 
    }else if(child>0 && gchild == 0){
	//second child
 	printf("Child Proces: C, returns: %d, pid: %d, ppid: %d\n",val, getpid(), getppid());
	exit(val);
    }else{
	//grandchild
	printf("Child Process: D, returns: %d, pid: %d, ppid: %d\n", val, getpid(), getppid());
	exit(val);
    }
    printf("Parent Process: A, returns: %d, pid: %d, ppid: %d\n", val, getpid(), getppid());
    return;
}

int main(){

processTree();

}

