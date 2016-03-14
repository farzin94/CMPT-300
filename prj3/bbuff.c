#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdbool.h>
#include "bbuff.h"

void* candyBuff[BUFFER_SIZE];
sem_t empty;
sem_t full;
sem_t mutex;
int emptyVal=0;

void bbuff_init(void){

}

void bbuff_blocking_insert(void* candyp){

	sem_wait(&mutex);
	sem_wait(&full);
	sem_getvalue(&empty, &emptyVal);
	candyBuff[emptyVal] = candyp;
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
	
	sem_wait(&mutex);
	sem_wait(&empty);  
	sem_post(&full);  
	sem_post(&mutex);
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
