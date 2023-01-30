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
	// A FAIRE
}

struct task_t * get_schedule_node_task(const struct schedule_node_t * snode) {
	// A FAIRE
}

unsigned long get_schedule_node_begin_time(const struct schedule_node_t * snode) {
	// A FAIRE
}

unsigned long get_schedule_node_end_time(const struct schedule_node_t * snode) {
	// A FAIRE
}

void set_schedule_node_begin_time(struct schedule_node_t * snode, unsigned long new_bt) {
	// A FAIRE
}

void set_schedule_node_end_time(struct schedule_node_t * snode, unsigned long new_et) {
	// A FAIRE
}

void view_schedule_node(const void * snode) {
	// A FAIRE
}

void delete_schedule_node(void * snode) {
	// A FAIRE
}

/********************************************************************
 * schedule_t
 ********************************************************************/

struct schedule_t * new_schedule(int num_m) {
	assert(num_m >= 1);
	// A FAIRE
}

struct list_t * get_schedule_of_machine(const struct schedule_t * S, const int machine) {
	// A FAIRE
}

int get_num_machines(const struct schedule_t * S) {
	// A FAIRE
}

void view_schedule(const struct schedule_t * S) {
	// A FAIRE
}

void delete_schedule(struct schedule_t * S) {
	// A FAIRE
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