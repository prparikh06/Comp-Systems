#include <unistd.h>
#include <sys/types.h> 
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "procTree.c"

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
	int sstatus;
      printf("Parent waiting for children to terminate...\n");

	waitpid(child,&status,0); // waiting on child B
      	child_num = WEXITSTATUS(status);	      
	printf("Process B returns  %d to Process A\n",child_num);

	ptr = createTree(_root, "Child B");
	ptr = createTree(_root->children[0], "Child D");

	waitpid(gchild,&sstatus,0); // waiting on Child C
	child_num = WEXITSTATUS(sstatus);

	printf("Process C returns  %d to Process A\n",child_num);
	ptr = createTree(_root, "Child: C");

	printf("All children have terminated\n");
    }else if(child == 0 && gchild>0){
      //first child
	//ptr = createTree(_root,"Child: B");
	int gstatus;
	printf("Child B waiting for grandchild to terminate\n");

	waitpid(gchild, &gstatus,0);
	child_num = WEXITSTATUS(gstatus);
	printf("Process D returns %d to Process B\n", child_num );

	exit(4); 
    }else if(child>0 && gchild == 0){
	//second child
	sleep(1);
	exit(6);
    }else{
	//grandchild
	exit(10);
    }
    printf("Parent Process returns\n");
    return;
}

int main(){

processTree();
print_tree(_root);
}

