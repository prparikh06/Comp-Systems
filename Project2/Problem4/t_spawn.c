#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<signal.h>
#include<errno.h>

#define handle_error_en(en,msg) \
	do {errno = en; perror(msg); exit(EXIT_FAILURE); } while(0)
long tidINT=0, tidSTOP=0, tidILL=0;

static void sig_int(){
	printf("Signal: 2 handled by TID: %li SIGINT!\n",tidINT);
}

static void sig_stop(){
	printf("Signal: 19 handled by TID: %li SIGSTOP!\n",tidSTOP);
	
}


static void sig_ill(){
	printf("Signal: 4 handled by TID: %li SIGILL \n",tidILL);
}


//thread function created to handle its SIGINT
void handle_sigINT() {
long i;
long long int sum;

pthread_t myID = pthread_self();

tidINT = (long)myID;


signal(SIGINT, sig_int);

	for(i = 0 ; i<(long)myID*1000 ; i++){
		sum = sum+i;
	}
printf("TID: %li, who handled SIGINT, finished its loop. Sum of 1 to %li is: %lli \n",(long)myID, (long)myID, sum);

}


//thread function made to count and handle its SIGSTOP
void handle_sigSTOP() {

pthread_t myID = pthread_self();
tidSTOP = (long)myID;
signal(SIGSTOP, sig_stop);
	
long i;
long long int sum;

	for(i = 0 ; i<(long)myID*1000 ; i++){
		sum = sum+i;
	}
	
	printf("TID: %li, who handled SIGSTOP, finished its loop. Sum of 1 to %li is: %lli \n",(long)myID, (long)myID, sum);

}

//thread created to count and handle its SIGILL
void handle_sigILL() {
long i;
long long int sum;
pthread_t myID = pthread_self();
tidILL = (long)myID;
signal(SIGILL, sig_ill);

	for(i = 0 ; i<(long)myID*1000 ; i++){
		sum = sum+i;
	}
printf("TID: %li, who handled SIGILL, finished its loop. Sum of 1 to %li is: %lli \n",(long)myID, (long)myID, sum);


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


sleep(2);


//send sigKILL
//s = pthread_kill(tid[0],SIGINT);
//if(s != 0)
//	handle_error_en(s,"Error Trying to send signal to SIGINT\n");

//send SIGSTOP
//s = pthread_kill(tid[1],SIGSTOP);
//if(s != 0)
//	handle_error_en(s,"Error Trying to send signal to SIGSTOP\n");

printf("TID SIGILL: %li; tid[2] pointer: %li\n",tidILL,(long)tid[2]);

//send SIGILL
s = pthread_kill(tid[2],SIGILL);
if(s != 0)
	handle_error_en(s,"Error Trying to send signal to SIGILL\n");





pthread_join(tid[0],NULL);
pthread_join(tid[1],NULL);
pthread_join(tid[2],NULL); 


return 0;
}
