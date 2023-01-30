#include "instance.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "util.h"
#include "list.h"

/********************************************************************
 * task_t
 ********************************************************************/

struct task_t * new_task(char * id, unsigned long proctime, unsigned long reltime) {
	// A FAIRE
}

char * get_task_id(const struct task_t * task) {
	// A FAIRE
}

unsigned long get_task_processing_time(const struct task_t * task) {
	// A FAIRE
}

unsigned long get_task_release_time(const struct task_t * task) {
	// A FAIRE
}

void view_task(const void * task) {
	// A FAIRE
}

void delete_task(void * task) {
	// A FAIRE
}

/********************************************************************
 * Instance
 ********************************************************************/

Instance read_instance(const char * filename) {
	// A FAIRE
}

void view_instance(Instance I) {
	// A FAIRE
}

void delete_instance(Instance I, int deleteData) {
	// A FAIRE
}