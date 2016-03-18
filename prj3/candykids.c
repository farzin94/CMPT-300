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

	int countProduced=0;
	while(!stop_thread){
		int rand_sec = rand()%3;
		
		printf("\tFactory %d ships candy & waits %ds\n", fac, rand_sec );
		
		candy_t* candy = malloc(sizeof(candy_t));
		candy->factory_number = fac;
		candy->time_stamp_in_ms = current_time_in_ms();
		
		
		bbuff_blocking_insert(candy);
		countProduced++;
		stats_record_produced(fac);

		sleep(rand_sec);	
		free(candy);
			
	}
	//printf("candy created by %d = %d\n", fac, countProduced);
	printf("Candy-factory %d done\n", fac);
	
}

void *kidFunc(void *p){
	
	srand(time(NULL));
	
	int countConsumed=0;
	while(1){
		
		candy_t* candy = bbuff_blocking_extract();
		if (candy){
			countConsumed++;
			stats_record_consumed(candy->factory_number, candy->time_stamp_in_ms);
		}
		
		sleep(rand()%1);
	}
	
	//printf("candy consumed from %d = %d\n", fac, count);
	return NULL;
}

int main(int argc, char *argv[]) {

	bbuff_init();
	stats_init(atoi(argv[1]));
	
	int factory=0;
	int kid=0;
	int sec=0;
	
	//-----1. Extract arguments-----------------
	if (argc == 0){
		printf("Error: Invalid input\n");
		exit(1);
	}

	if (argc > 0 && argc <= 4){
		factory = atoi(argv[1]);
		kid = atoi(argv[2]);
		sec = atoi(argv[3]);
	}
	
	if (factory <= 0 || kid <= 0 || sec <= 0){
		printf("Error: Invalid input\n");
		exit(1);
	}
	
	 //-----2. Initialize modules-------------------

	
	//------3. Launch candy-factory threads-----------
	
	pthread_t factory_thread[factory]; //to hold factory threads
	int factory_num[factory]; //to pass factory number to factoryFunc
	
	for (int k=0; k<factory; k++){
		factory_num[k] = k;
	}
	
	for (int i = 0; i < factory; i++) {
		pthread_attr_t attr;
        pthread_attr_init(&attr);
		if(pthread_create(&factory_thread[i],&attr, factoryFunc, &factory_num[i])) {
			printf ("Create pthread error!\n");
			exit (1);
		}
	}
	
	pthread_t kid_thread[kid];
	
	//------4. Launch kid threads-------------------
	for (int i = 0; i < kid; i++) {
		pthread_attr_t attr;
        pthread_attr_init(&attr);
		if(pthread_create(&kid_thread[i], &attr, kidFunc, NULL)) {
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
	stop_thread = true;
	for (int x=0; x<factory; x++){
		pthread_join(factory_thread[x],  NULL);
	}
	
	//------7. Wait until no more candy--------------
	while (bbuff_is_empty() == false){
		printf("Waiting for all candy to be consumed\n");
		sleep(1);
	}
	
	printf("bye\n");
	//------8. Stop kid threads----------------------
	for (int i = 0; i < kid; i++) {
		pthread_cancel(kid_thread[i]);
		pthread_join(kid_thread[i], NULL);
	}

	//------9. Print statistics-----------------
	stats_display();
	
	//------10. Cleanup any allocated memory---------
	stats_cleanup();
	
	return 0;
}