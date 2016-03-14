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

void bbuff_init(void){
	sem_init(&empty, 0, 0); // MAX buffers are empty to begin with...
	sem_init(&full, 0, BUFFER_SIZE);    // ... and 0 are full
	sem_init(&mutex, 0, 1);
}

void bbuff_blocking_insert(void* candyp){

	//Should emptyVal be incremented?
	sem_wait(&mutex);
	sem_wait(&full);
	sem_getvalue(&empty, &emptyVal);
	candyBuff[emptyVal] = candyp;
	printf("emptyVal: %d\n", emptyVal);
	sem_post(&empty);
	sem_post(&mutex);
	
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
	
	//is item being dequeued?
	sem_wait(&mutex);
	void* candyp = candyBuff[emptyVal];
	sem_wait(&empty); 
	
	
	//candyBuff[emptyVal] = 
	sem_post(&full);  
	sem_post(&mutex);
	
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
	sem_getvalue(&empty, &emptyVal);
	if (emptyVal > 0)
		{
			return false;
		}
	return true;
}
