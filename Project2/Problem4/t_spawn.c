#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

#define handle_error_en(en,msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)
int tidINT=0, tidSTOP=0, tidILL=0;

static void sig_int(){
	printf("TID: %d handled this SIGINT!\n",tidINT);
}

static void sig_stop(){
	printf("TID: %d handled this SIGSTOP!\n",tidSTOP);
}


static void sig_ill(){
	printf("TID: %d handled this SIGILL!! \n",tidILL);
}


//thread function created to handle its SIGINT
void handle_sigINT() {
int sum,i;
pthread_t myID = pthread_self();

tidINT = (int)myID;


signal(SIGINT, sig_int);

	for(i = 0 ; i<(int)myID ; i++){
		sum = sum+i;
	}
printf("TID: %d, who handled SIGINT, finished its loop. Sum of 1 to %d is: %d \n",(int)myID, (int)myID, sum);

}


//thread function made to count and handle its SIGSTOP
void handle_sigSTOP() {

pthread_t myID = pthread_self();
tidSTOP = (int)myID;
signal(SIGSTOP, sig_stop);
	
int i, sum;

	for(i = 0 ; i<(int)myID ; i++){
		sum = sum+i;
	}
	
	printf("TID: %d, who handled SIGSTOP, finished its loop. Sum of 1 to %d is: %d \n",(int)myID, (int)myID, sum);

}

//thread created to count and handle its SIGILL
void handle_sigILL() {
int sum,i;
pthread_t myID = pthread_self();
tidILL = (int)myID;
signal(SIGILL, sig_int);

	for(i = 0 ; i<(int)myID ; i++){
		sum = sum+i;
	}
printf("TID: %d, who handled SIGILL, finished its loop. Sum of 1 to %d is: %d \n",(int)myID, (int)myID, sum);


}


int main() {

pthread_t tid[3]; //initialize 3 tid variables
sigset_t mask; //create a signal mask
int s; //get return of signal masks

//make mask to handle SIGINT
sigemptyset(&mask);
sigaddset(&mask, SIGINT);
//sigprocmask(SIG_SETMASK,&mask, NULL); 

//mask for pthread
s = pthread_sigmask(SIG_BLOCK, &mask, NULL);

if(s != 0)
	handle_error_en(s,"Error with creating signal mask for SIGINT.\n");
//create thread to handle SIGINT
s = pthread_create(&tid[0], NULL, (void *) handle_sigINT,NULL);

if(s != 0)
	handle_error_en(s,"Error creating thread to handle SIGINT\n");

//repeat above, but for SIGSTOP
sigemptyset(&mask); //empty mask set
sigaddset(&mask, SIGSTOP);
//sigprocmask(SIG_SETMASK,&mask, NULL); 

s = pthread_sigmask(SIG_BLOCK, &mask, NULL);

if(s != 0)
	handle_error_en(s,"Error with creating signal mask for SIGSTOP.\n");

s = pthread_create(&tid[1], NULL, (void *) handle_sigSTOP,NULL);

if(s != 0)
	handle_error_en(s,"Error creating thread to handle SIGSTOP\n");


//same as above, but now for SIGILL
sigemptyset(&mask);
sigaddset(&mask, SIGILL);
//sigprocmask(SIG_SETMASK,&mask, NULL); 

s = pthread_sigmask(SIG_BLOCK, &mask, NULL);

if(s != 0)
	handle_error_en(s,"Error with creating signal mask for SIGILL.\n");

s = pthread_create(&tid[2], NULL, (void *) handle_sigILL,NULL);

if(s != 0)
	handle_error_en(s,"Error creating thread to handle SIGILL.\n");


//start accepting signals again 
sigemptyset(&mask);
sigprocmask(SIG_SETMASK, &mask, NULL);


sleep(1);

//s = pthread_kill(tid[0],SIGINT);
//if(s != 0)
//	handle_error_en(s,"Error Trying to send signal to SIGINT\n");

//s = pthread_kill(tid[1],SIGSTOP);
//if(s != 0)
//	handle_error_en(s,"Error Trying to send signal to SIGSTOP\n");

s = pthread_kill(tid[2],SIGILL);
if(s != 0)
	handle_error_en(s,"Error Trying to send signal to SIGILL\n");





pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
pthread_join(tid[2],NULL); 


return 0;
}
