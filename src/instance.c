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
	struct task_t * task = (struct task_t *) malloc(sizeof(struct task_t));
    task->id = (char *) malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(task->id, id);
	task->processing_time = proctime;
	task->release_time = reltime;
	return task;
}

char * get_task_id(const struct task_t * task) {
	return task->id;
}

unsigned long get_task_processing_time(const struct task_t * task) {
	return task->processing_time;
}

unsigned long get_task_release_time(const struct task_t * task) {
	return task->release_time;
}

void view_task(const void * task) {
	printf("================\n Task %s\n, processing time %lu\n, release time %lu\n========================\n", get_task_id(task), get_task_processing_time(task), get_task_release_time(task));
}

void delete_task(void * task) {
	assert(task);
	struct task_t * t = (struct task_t *) task;
	free(t->id);
	free(t);
}

/********************************************************************
 * Instance
 ********************************************************************/

Instance read_instance(const char * filename) {
	//Ouvre le fichier
	FILE * file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Erreur : Impossible d'ouvrir le fichier %s\n", filename);
		exit(EXIT_FAILURE);
	}
	// Initialise une nouvelle instance 
	Instance I = new_list(sizeof(struct task_t *), view_task, delete_task());
	// Lit le fichier ligne par ligne
	char * line = NULL;// chaine de caractère pour stocker chaque ligne du fichier
	size_t len = 0;// taille initiale de la chaine de caractère(0 pour que getline alloue la mémoire)
	ssize_t read;// nombre de caractère lu par la ligne courante
	char *token;// chaine de caractère pour stocker chaque partie de la ligne (id, processing time, release time)
	char delim= ' ';//Délimiteur
	while ((read = fgets(&line, &len, file)) != -1) {
		//découpe la ligne en 3 parties,
		token=strtok(line,&delim);
		char *id=token;
		token=strtok(NULL,&delim);
		unsigned long processing_time=strtoul(token,NULL,10);
		token=strtok(NULL,&delim);
		unsigned long release_time=strtoul(token,NULL,10);
		//crée une nouvelle tâche avec ces 3 parties et l'ajoute à l'instance
		struct task_t * task = new_task(id, processing_time, release_time);
		add_to_list(I, task);
	}
	//Libère la mémoire
	free(line);
	fclose(file);
	return I;
}

void view_instance(Instance I) {
	printf("Instance avec %d tache(s) : \n"), get_list_size(I);
	struct list_node_t * node = I->head;
	while (node) {
		i->viewData(node->data);
		node = node->next;
	}
}

void delete_instance(Instance I, int deleteData) {
	assert(i);
	struct list_node_t * node = I->head;
	while (node) {
		struct list_node_t * next = node->next;
		if (deleteData) {
			i->deleteData(node->data);
		}
		free(node);
		node = next;
	}
	free(i);
}