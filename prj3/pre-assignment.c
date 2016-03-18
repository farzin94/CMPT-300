#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>

pthread_mutex_t lock;
int num=0;

void* numOne(void *p){
	
	pthread_mutex_lock(&lock);
	while (num) {
		num=0;
		//printf("thread1: %d ", num);
	}
	pthread_mutex_unlock(&lock);
	
	return NULL;
}

void* numZero(void *p){
	
	pthread_mutex_lock(&lock);
	
	while(!num) {
		num=1;
		//printf("thread2: %d ", num);
	}
	pthread_mutex_unlock(&lock);
	
	return NULL;
}

int main(){
	
	pthread_t thread1;
	pthread_t thread2;
	
	pthread_create(&thread1, NULL, numOne , NULL);
	pthread_create(&thread2, NULL, numZero , NULL);
	
	//sleep(1);
	
	pthread_join(thread1, NULL);
	pthread_join(thread2, NULL);
	return 0;
}
/**
 * Two threads alternate using a shared integer num (set to
be 0 initially), a mutex lock and two condition variables.
Thread #1 keeps waiting for num to become 1 and then
changing it to 0. Thread #1 keeps waiting for num to
become 0 and changing it to 1.
◦ Figure out the rest





 * 1. real	0m0.002s
 * user	0m0.000s
 * sys	0m0.000s
 * 
 */
