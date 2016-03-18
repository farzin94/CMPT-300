#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include "bbuff.h"

void* candyBuff[BUFFER_SIZE];
sem_t empty;
sem_t full;
sem_t mutex;
int emptyVal;
//int fullVal;
//int count = 0;

void bbuff_init(void){
	sem_init(&empty, 0, 0); // MAX buffers are empty to begin with...
	sem_init(&full, 0, BUFFER_SIZE);    // ... and 0 are full
	sem_init(&mutex, 0, 1);
}

void bbuff_blocking_insert(void* candyp){
	
	sem_wait(&full);
	sem_wait(&mutex);
//count++;
//printf("Count: %d\n", count);
sem_getvalue(&empty, &emptyVal);
/*sem_getvalue(&full, &fullVal);
printf("fullVal: %d\n", fullVal);*/
printf("emptyVal insert: %d\n", emptyVal);
	candyBuff[emptyVal] = candyp;
	sem_post(&mutex);
	sem_post(&empty);
	
	
}
	
	

/*Producer(item) {
 emptyBuffers.P(); // Wait until space
 mutex.P(); // Wait until machine free
Enqueue(item);
 mutex.V();
 fullBuffers.V(); // Tell consumers there is
 // more coke
}*/


void* bbuff_blocking_extract(void){
	
	void* candyp;
		
	sem_wait(&empty); 
	sem_wait(&mutex);
	sem_getvalue(&empty, &emptyVal);
/*sem_getvalue(&full, &fullVal);
printf("fullVal EXTRACT: %d\n", fullVal);*/
printf("emptyVal EXTRACT: %d\n", emptyVal);
//count--;
//printf("Count EXTRACT: %d\n", count);
	candyp = candyBuff[emptyVal];
	candyBuff[emptyVal] = NULL;
	sem_post(&mutex);
	sem_post(&full);  
	
	return candyp;
}
/*Consumer() {
 fullBuffers.P(); // Check if there’s a coke
 mutex.P(); // Wait until machine free
item = Dequeue();
 mutex.V();
 emptyBuffers.V(); // tell producer need more
 return item;
}
*/

/**
 * Returns true when bounded buffer is empty
 */
_Bool bbuff_is_empty(void){
	sem_wait(&mutex);
	sem_getvalue(&empty, &emptyVal);
	sem_post(&mutex);
	if (emptyVal > 0)
	{
		return false;
	}
	return true;
}