#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include "bbuff.h"

void* candyBuff[BUFFER_SIZE];
sem_t empty;
sem_t full;
sem_t mutex;
int in = 0;
int out = 0;
int count = 0;
int emptyVal;

void bbuff_init(void){
	sem_init(&empty, 0, BUFFER_SIZE); // MAX buffers are empty to begin with...
	sem_init(&full, 0, 0);    // ... and 0 are full
	sem_init(&mutex, 0, 1);
}

void bbuff_blocking_insert(void* candyp){

	//Should emptyVal be incremented?
	sem_wait(&empty);
	sem_wait(&mutex);
	count++;
	//sem_getvalue(&empty, &emptyVal);
	candyBuff[in] = candyp;
	in = (in+1) % BUFFER_SIZE;
	//printf("emptyVal: %d\n", emptyVal);
	sem_post(&mutex);
	sem_post(&full);
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
	sem_wait(&full); 
	sem_wait(&mutex);
	count--;
	candyp = candyBuff[out];
	candyBuff[out] == NULL;
	out = (out+1) % BUFFER_SIZE;
	sem_post(&mutex);
	sem_post(&empty);  
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
	//sem_getvalue(&empty, &emptyVal);
	if (count > 0)
		{
			return false;
		}
	return true;
}