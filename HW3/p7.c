#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t c_lock, b_lock;
pthread_cond_t c_ready = PTHREAD_COND_INITIALIZER, b_ready = PTHREAD_COND_INITIALIZER;
int espresso_count = 0;
int c_ticket = 0;

int numBaristas;

void * customer(void * args)
{
	pthread_mutex_lock(&c_lock);
	c_ticket++;
	int ticket = c_ticket;
	pthread_mutex_unlock(&c_lock);
	
	pthread_cond_broadcast(&c_ready);
	
	pthread_mutex_lock(&b_lock);
	while (ticket > espresso_count)
	{
		pthread_cond_wait(&b_ready, &b_lock);
	}
	pthread_mutex_unlock(&b_lock);
	
	printf("I am customer placing ticket %i.\n", ticket);
}

void * barista(void * args)
{
	while (1)
	{
		pthread_mutex_lock(&b_lock);
		espresso_count++;
		int ticket = espresso_count;
		pthread_mutex_unlock(&b_lock);
		
		sleep(1);
		pthread_cond_broadcast(&b_ready);
		
		pthread_mutex_lock(&c_lock);
		while (c_ticket < ticket)
		{
			pthread_cond_wait(&c_ready, &c_lock);
		}
		pthread_mutex_unlock(&c_lock);
		
		printf("I am barista serving ticket %i.\n", ticket);
	}
}

int main(int argc, char **argv)
{
	numBaristas = atoi(argv[1]);
	int numCustomers = atoi(argv[2]);
	
	pthread_t bid[numBaristas];
	pthread_t cid[numCustomers];
	
	if (pthread_mutex_init(&c_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 
	if (pthread_mutex_init(&b_lock, NULL) != 0) { 
        printf("\n mutex init has failed\n"); 
        return 1; 
    } 

	int i;
	for (int i = 0; i < numBaristas; i++)
	{
		pthread_create(&bid[i], NULL, barista, NULL);
	}
	
	for (int i = 0; i < numCustomers; i++)
	{
		pthread_create(&cid[i], NULL, customer, NULL);
	}
	
	// this will never happen unless we figure out a better loop condition for the barristas
	for (int i = 0; i < numBaristas; i++)
	{
		pthread_join(bid[i], NULL);
	}
	for (int i = 0; i < numCustomers; i++)
	{
		pthread_join(cid[i], NULL);
	}
	
	pthread_mutex_destroy(&c_lock); 
	pthread_mutex_destroy(&b_lock); 
}






