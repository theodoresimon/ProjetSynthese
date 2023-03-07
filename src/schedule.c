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
	if(snode != NULL) {
		free(snode);
	}
}

/********************************************************************
 * schedule_t
 ********************************************************************/

struct schedule_t * new_schedule(int num_m) {
	assert(num_m >= 1);
	struct schedule_t * S = (struct schedule_t *) malloc(sizeof(struct schedule_t));
	for(int i = 0; i < num_m; i++){
		S->machines[i] = new_list(view_schedule_node, delete_schedule_node);
	}
	S->num_machines = num_m;
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
	for(int i = 0; i < S->num_machines; i++){
		printf("Machine n°%d", i);
		view_list(S->machines[i]);
	}
}

void delete_schedule(struct schedule_t * S) {
	for(int i = 0; i < S->num_machines; i++){
		delete_list(S->machines[i]);
	}
	free(S->machines);
}

// Pour le format du fichier à créer, regardez dans la présentation du cours.
void save_schedule(struct schedule_t * S, char * filename) {
	assert(S == NULL);
	assert(filename == NULL);
	FILE * file = fopen(filename, "w");
	if(file != NULL){
		for(int i = 0; i < S->num_machines; i++){
			struct list_t * list = S->machines[i];
			struct schedule_node_t * snode = get_list_head(list);
			while(snode != NULL){
				fprintf(file, "%d %lu %lu\n", get_task_id(get_schedule_node_task(snode)), get_schedule_node_begin_time(snode), get_schedule_node_end_time(snode));
				snode = get_list_next(list, snode);
			}
		}
		fclose(file);
	}
}

// Trouver la machine qui est vide
int find_empty_machine(struct schedule_t * S, unsigned long time) {
	assert S == NULL;
	assert time < 0;
	for(int i = 0; i < S->num_machines; i++){
		struct list_t * list = S->machines[i];// récupère la liste de la machine i
		struct schedule_node_t * snode = get_list_tail(list);// récupère le dernier élément de la liste
		if(snode == NULL){// si la liste est vide
			return i;// on retourne la machine i
		}
	}
	return -1;// si aucune machine n'est vide, on retourne -1
}

// Trouver la machine qui a fini son travail le plus tôt
int find_machine_to_interrupt(struct schedule_t * S, unsigned long time, unsigned long processing_time) {
	assert S == NULL;
	assert time < 0;
	assert processing_time < 0;
	for(int i = 0; i < S->num_machines; i++){
		struct list_t * list = S->machines[i];// récupère la liste de la machine i
		struct schedule_node_t * snode = get_list_tail(list);// récupère le dernier élément de la liste
		if(snode != NULL){
			if(snode->end_time <= time - get_schedule_node_begin_time(snode)){// si la fin du dernier élément de la liste est inférieur ou égale au temps actuel moins le début du dernier élément de la liste
				return i;
			}
		}
	}
	return -1;
}

void add_task_to_schedule(struct schedule_t * S, struct task_t * task, int machine, unsigned long bt, unsigned long et) {
	assert(S == NULL);
	assert(task == NULL);
	assert(machine < 0 || machine >= S->num_machines);
	assert(bt < et);
	struct schedule_node_t * snode = new_schedule_node(task, bt, et);
	if(snode != NULL){
		add_to_list(S->machines[machine], snode);
	}
}

unsigned long preempt_task(struct schedule_t * S, int machine, unsigned long new_et) {
	S->machines[machine]->tail->end_time = new_et;
	return new_et;
}

unsigned long get_makespan(struct schedule_t * S) {
	assert(S == NULL);
	unsigned long makespan = 0;
	for(int i = 0; i < S->num_machines; i++){
		struct list_t * list = S->machines[i];
		struct schedule_node_t * snode = get_list_tail(list);
		if(snode != NULL){
			unsigned long end_time = get_schedule_node_end_time(snode);
			if(end_time > makespan){
				makespan = end_time;
			}
		}
	}
	return makespan;
}