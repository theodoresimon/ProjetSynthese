#include "schedule.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "list.h"
#include "instance.h"

/********************************************************************
 * schedule_node_t
 ********************************************************************/

/**
 * @brief Construire et initialiser un nouveau nœud d'un ordonnancement.
 * 
 * @param[in] task 
 * @param[in] bt 
 * @param[in] et 
 * @return struct schedule_node_t* 
 */
static struct schedule_node_t * new_schedule_node(struct task_t * task, unsigned long bt, unsigned long et) {
	assert(bt < et);
	if (task == NULL) {
		return NULL;
	}
	struct schedule_node_t * snode = (struct schedule_node_t *) malloc(sizeof(struct schedule_node_t));
	if (snode == NULL) {
		return NULL;
	}
	snode->task = task;
	snode->begin_time = bt;
	snode->end_time = et;
	return snode;
}

struct task_t * get_schedule_node_task(const struct schedule_node_t * snode) {
	return snode->task;
}

unsigned long get_schedule_node_begin_time(const struct schedule_node_t * snode) {
	return snode->begin_time;
}

unsigned long get_schedule_node_end_time(const struct schedule_node_t * snode) {
	return snode->end_time;
}

void set_schedule_node_begin_time(struct schedule_node_t * snode, unsigned long new_bt) {
	snode->begin_time = new_bt;
}

void set_schedule_node_end_time(struct schedule_node_t * snode, unsigned long new_et) {
	snode->end_time = new_et;
}

void view_schedule_node(const void * snode) {
	printf("Task %d \n, begin time %lu\n, end time %lu\n)", get_task_id(get_schedule_node_task(snode)), get_schedule_node_begin_time(snode), get_schedule_node_end_time(snode));
}

void delete_schedule_node(void * snode) {
	if(snode == NULL)
		
	free(snode);
}

/********************************************************************
 * schedule_t
 ********************************************************************/

struct schedule_t * new_schedule(int num_m) {
	assert(num_m >= 1);
	struct schedule_t * S = (struct schedule_t *) malloc(sizeof(struct schedule_t));
	if (S == NULL) {
		return NULL;
	}
	S->num_machines = num_m;
	S->machines = (struct list_t **) malloc(num_m * sizeof(struct list_t *));
	if (S->machines == NULL) {
		free(S);
		return NULL;
	}
	for (int i = 0; i < num_m; i++) {
		S->machines[i] = new_list();
		if (S->machines[i] == NULL) {
			for (int j = 0; j < i; j++) {
				delete_list(S->machines[j]);
			}
			free(S->machines);
			free(S);
			return NULL;
		}
	}
	return S;
}

struct list_t * get_schedule_of_machine(const struct schedule_t * S, const int machine) {
	assert(machine >= 0 && machine < S->num_machines);
	return S->machines[machine];
}

int get_num_machines(const struct schedule_t * S) {
	return S->num_machines;
}

void view_schedule(const struct schedule_t * S) {
	printf("Schedule: \n");
}

void delete_schedule(struct schedule_t * S) {
	for(int i = 0; i < S->num_machines; i++){
		delete_list(S->machines[i]);
	}
	free(S->machines);
}

// Pour le format du fichier à créer, regardez dans la présentation du cours.
void save_schedule(struct schedule_t * S, char * filename) {
	// A FAIRE
}


int find_empty_machine(struct schedule_t * S, unsigned long time) {
	// A FAIRE
}

int find_machine_to_interrupt(struct schedule_t * S, unsigned long time, unsigned long processing_time) {
	// A FAIRE
}

void add_task_to_schedule(struct schedule_t * S, struct task_t * task, int machine, unsigned long bt, unsigned long et) {
	// A FAIRE
}

unsigned long preempt_task(struct schedule_t * S, int machine, unsigned long new_et) {
	// A FAIRE
}

unsigned long get_makespan(struct schedule_t * S) {
	// A FAIRE
}