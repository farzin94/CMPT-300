#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <semaphore.h>

#include "bbuff.c"
#include "stats.c"

/**
 * REMINDER: ARGUMENTS GO IN THE COMMAND LINE OR YOU SEGFAULT
 */

_Bool stop_thread = false;

typedef struct {
   int factory_number;
   double time_stamp_in_ms;
} candy_t;

void *factoryFunc(void *p){
	
	int fac = *((int *) p);
	srand(time(NULL));
	while(!stop_thread){
		int ran_sec = rand()%3;
		
		printf("\tFactory %d ships candy & waits %ds\n", fac, ran_sec );
		sleep(ran_sec);
		
	}
	printf("Candy-factory %d done\n", fac);
	//printf ("fac: %d\n", fac);
	
	return NULL;
}

void *kidFunc(void *p){
	return NULL;
}

int main(int argc, char *argv[]) {

	int factory=0;
	int kid=0;
	int sec=0;
	sem_init(&empty, 0, 0); // MAX buffers are empty to begin with...
	sem_init(&full, 0, BUFFER_SIZE);    // ... and 0 are full
	sem_init(&mutex, 0, 1);
	
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
	
	printf("%d %d %d\n", factory , kid , sec);
	pthread_t factory_thread[factory];
	int factory_num[factory];
	
	for (int i=0; i<factory; i++){
		factory_num[i] = i;
		//printf("factory_num: %d\n", factory_num[i]);
	}
		
	for (int i = 0; i < factory; i++) {
		if(pthread_create(&factory_thread[i], NULL, factoryFunc, &factory_num[i])) {
			printf ("Create pthread error!\n");
			exit (1);
		}
	}
	
	pthread_t kid_thread[kid];
	//int factory_num[factory];
	
	for (int i = 0; i < kid; i++) {
		if(pthread_create(&kid_thread[i], NULL, kidFunc, NULL)) {
			printf ("Create pthread error!\n");
			exit (1);
		}
	}
	
	
	for (int p=1; p<sec; p++){
		sleep(1);
		printf("Time %ds\n", p);
	}
	
	stop_thread = true;
	
	for (int x=0; x<factory; x++){
		pthread_join(factory_thread[x], NULL);
	}
	
	

 // 2. Initialize modules
	
 // 3. Launch candy-factory threads
 // 4. Launch kid threads
 // 5. Wait for requested time
 // 6. Stop candy-factory threads
 // 7. Wait until no more candy
 // 8. Stop kid threads
 // 9. Print statistics

 // 10. Cleanup any allocated memory
}