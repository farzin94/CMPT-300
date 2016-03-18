#include "stats.h"
#include <float.h>
/**
 * [stats_init description]
 * @param num_producers [description]
 */
typedef struct {
	int candyMade;
	int candyEaten;
	double tot_delay;
	double max_delay;
	double min_delay;
} candy_fact_stat;

candy_fact_stat* stats;
int global_num_producers;

void stats_init(int num_producers){
	global_num_producers = num_producers;
	stats = malloc(sizeof(candy_fact_stat)*num_producers);
	
	for (int i = 0; i < num_producers; i++)
	{
		stats[i].candyMade = 0;
		stats[i].candyEaten = 0;
		stats[i].tot_delay = 0;
		stats[i].max_delay= 0;
		stats[i].min_delay= DBL_MAX;
	}
}

/**
 * [stats_cleanup description]
 */
void stats_cleanup(void){

	if (stats) free(stats);
}

/**
 * [stats_record_produced description]
 * @param factory_number [description]
 */
void stats_record_produced(int factory_number){
	stats[factory_number].candyMade++;
}

/**
 * [stats_record_consumed description]
 * @param factory_number [description]
 * @param delay_in_ms    [description]
 */
void stats_record_consumed(int factory_number, double delay_in_ms){
	printf("DELAY %f\n", delay_in_ms);
	stats[factory_number].candyEaten++;
	stats[factory_number].tot_delay = stats[factory_number].tot_delay + delay_in_ms;
	
	if (delay_in_ms > stats[factory_number].max_delay)
	{
		stats[factory_number].max_delay = delay_in_ms;
	}
	if (delay_in_ms < stats[factory_number].min_delay)
	{
		stats[factory_number].min_delay = delay_in_ms;
	}
	
}

/**
 * [stats_display description]
 */
void stats_display(void){
	double avg_delay[global_num_producers]; 

	for (int k = 0; k <global_num_producers; k++)
	{
		if (stats[k].min_delay == DBL_MAX)
		{
			stats[k].min_delay = 0;
		}
		if (stats[k].candyEaten != 0)
		{
			avg_delay[k] = (stats[k].tot_delay) / (stats[k].candyEaten);
		}
		else
		{
			avg_delay[k] = 0;
		}
	}
	//Title Row
	printf("%8s%10s%30s%30s%30s%30s\n", "Factory#", "Made", "#Eaten", "Min Delay[ms]", "Avg Delay[ms]", "Max Delay[ms]");
	//Data Row
	for (int i = 0; i<global_num_producers; i++)
	{
		printf("%8d%10d%30d%30f%30f%30f\n", i, stats[i].candyMade, stats[i].candyEaten, stats[i].min_delay, avg_delay[i], stats[i].max_delay);
	}
	for (int j = 0; j < global_num_producers; j++)
	{
		if (stats[j].candyMade != stats[j].candyEaten){
			printf("Candy Made does not match Candy Eaten on factory: %d\n", j);
		}
	}
}
