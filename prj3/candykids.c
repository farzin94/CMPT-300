#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>

#include "bbuff.c"
#include "stats.c"

//#define _POSIX_C_SOURCE 199609L

//struct timespec now; 
/**
 * REMINDER: ARGUMENTS GO IN THE COMMAND LINE OR YOU SEGFAULT
 */

_Bool stop_thread = false;


typedef struct {
   int factory_number;
   double time_stamp_in_ms;
} candy_t;



double current_time_in_ms(void){
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	
	return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}


void *factoryFunc(void *p){
	
	int fac = *((int *) p);
	srand(time(NULL));
	while(stop_thread==false){
		
		int rand_sec = rand()%3;
		
		printf("\tFactory %d ships candy & waits %ds\n", fac, rand_sec );
		
		candy_t* candy = malloc(sizeof(candy_t));
		candy->factory_number = fac;
		candy->time_stamp_in_ms = current_time_in_ms();
		
		//printf("time: %f\n", candy->time_stamp_in_ms);
		
		bbuff_blocking_insert(candy);
		
		
		
		sleep(rand_sec);	
		free(candy);
	}
	//assert(!bbuff_is_empty());
	printf("Candy-factory %d done\n", fac);
	//printf ("fac: %d\n", fac);
	
	//return candy;
}

void *kidFunc(void *p){
	
	srand(time(NULL));
	
	while(1){
		
		//printf("*\n");
		candy_t* candy = bbuff_blocking_extract();
			//Process ?
		printf("time: %f\n", candy->time_stamp_in_ms);
		 
		sleep(rand()%1);
		
	}
	
	
	//return NULL;
}

int main(int argc, char *argv[]) {

	int factory=0;
	int kid=0;
	int sec=0;
	
	
	
	assert(bbuff_is_empty());
	//-----1. Extract arguments-----------------
	if (argc == 0){
		printf("Error: Invalid input\n");
	}

	if (argc > 0 && argc <= 4){
		factory = atoi(argv[1]);
		kid = atoi(argv[2]);
		sec = atoi(argv[3]);
	}
	
	if (factory <= 0 || kid <= 0 || sec <= 0){
		printf("Error: Invalid input\n");
	}
	
	printf("factory: %d, kids: %d, seconds: %d\n", factory , kid , sec);
	
	 //-----2. Initialize modules-------------------
	
	bbuff_init();
	
	/*candy_t* candy = malloc(sizeof(candy_t));
		candy->factory_number = 0;
		candy->time_stamp_in_ms = current_time_in_ms();
		
		//printf("time: %f\n", candy->time_stamp_in_ms);
		
		bbuff_blocking_insert(candy);*/
	
	pthread_t factory_thread[factory]; //to hold factory threads
	int factory_num[factory]; //to pass factory number to factoryFunc
	
	for (int i=0; i<factory; i++){
		factory_num[i] = i;
		//printf("factory_num: %d\n", factory_num[i]);
	}
		
	//------3. Launch candy-factory threads-----------
	for (int i = 0; i < factory; i++) {
		if(pthread_create(&factory_thread[i], NULL, factoryFunc, &factory_num[i])) {
			printf ("Create pthread error!\n");
			exit (1);
		}
	}
	
	pthread_t kid_thread[kid];
	
	//------4. Launch kid threads-------------------
	for (int i = 0; i < kid; i++) {
		if(pthread_create(&kid_thread[i], NULL, kidFunc, NULL)) {
			printf ("Create pthread error!\n");
			exit (1);
		}
	}
	
	//------5. Wait for requested time------------
	for (int p=1; p<sec; p++){
		sleep(1);
		printf("Time %ds\n", p);
	}
	
	//------6. Stop candy-factory threads-----------
	
	for (int x=0; x<factory; x++){
		stop_thread = true;
		pthread_join(factory_thread[x], NULL);
	}
	
	//------7. Wait until no more candy--------------
	while (!bbuff_is_empty()){
		printf("Waiting for all candy to be consumed");
		sleep(1);
	}
	
	//------8. Stop kid threads----------------------
	for (int i = 0; i < kid; i++) {
		pthread_cancel(kid_thread[i]);
		pthread_join(kid_thread[i], NULL);
	}

	//------9. Print statistics-----------------
	//------10. Cleanup any allocated memory---------
}