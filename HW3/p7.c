#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t c_lock, b_lock;
pthread_cond_t c_avail, b_avail;
int espresso_count = 0;
int c_ticket = 0;

int numCustomers, numBaristas;

void * customer(void * args)
{
	pthread_mutex_lock(&c_lock);
	c_ticket++;
	int ticket = c_ticket;
	pthread_mutex_unlock(&c_lock);
	
	pthread_cond_broadcast(&c_avail);
	
	pthread_mutex_lock(&b_lock);
	while (ticket > espresso_count)
	{
		pthread_cond_wait(&b_avail, &b_lock);
	}
	printf("I am customer placing ticket %i.\n", ticket);
	pthread_mutex_unlock(&b_lock);
}

void * barista(void * args)
{
	int *bnum = (int*)args;
	
	pthread_mutex_lock(&b_lock);
	int cond = (espresso_count < numCustomers);
	pthread_mutex_unlock(&b_lock);
	while (cond)
	{
		pthread_mutex_lock(&b_lock);
		espresso_count++;
		int ticket = espresso_count;
		pthread_mutex_unlock(&b_lock);
		
		pthread_cond_broadcast(&b_avail);
		
		pthread_mutex_lock(&c_lock);
		while (c_ticket < ticket)
		{
			pthread_cond_wait(&c_avail, &c_lock);
		}
		printf("I am barista %i serving ticket %i.\n", *bnum, ticket);
		pthread_mutex_unlock(&c_lock);
		
		pthread_mutex_lock(&b_lock);
		cond = (espresso_count < numCustomers);
		pthread_mutex_unlock(&b_lock);
	}
	free(bnum);
}

int main(int argc, char **argv)
{
	numBaristas = atoi(argv[1]);
	numCustomers = atoi(argv[2]);
	
	pthread_t bid[numBaristas];
	pthread_t cid[numCustomers];
	
	if (pthread_mutex_init(&c_lock, NULL) != 0) 
	{ 
        printf("Failed to init mutex\n"); 
        return 1; 
    } 
	if (pthread_mutex_init(&b_lock, NULL) != 0) 
	{ 
        printf("Failed to init mutex\n"); 
        return 1; 
    } 

	int i;
	for (int i = 0; i < numBaristas; i++)
	{
		int *bnum = malloc(sizeof(int));
		*bnum = i;
		pthread_create(&bid[i], NULL, barista, (void *) bnum);
	}
	
	for (int i = 0; i < numCustomers; i++)
	{
		pthread_create(&cid[i], NULL, customer, NULL);
	}
	

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






