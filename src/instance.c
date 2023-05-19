#include "instance.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include <unistd.h>
#include "util.h"
#include "list.h"

/********************************************************************
 * task_t
 ********************************************************************/

struct task_t * new_task(char * id, unsigned long proctime, unsigned long reltime) {
	struct task_t * task = (struct task_t *) malloc(sizeof(struct task_t));
	task->id = id;
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
	free(task);
}

/********************************************************************
 * Instance
 ********************************************************************/

Instance read_instance(const char * filename) {
	assert(filename);//vérifie que le nom du fichier n'est pas null
	//Ouvre le fichier
	FILE *file = fopen(filename, "r");//ouvre le fichier en lecture
	if (file) {//si l'ouverture a échoué
		ShowMessage("impossible d'ouvrir le fichier",1);
	}
	// Initialise une nouvelle instance 
	Instance I = new_list(view_task, delete_task);
	// Lit le fichier ligne par ligne
	char * line = NULL;// chaine de caractère pour stocker chaque ligne du fichier
	size_t len = 0;// taille initiale de la chaine de caractère(0 pour que getline alloue la mémoire)
	ssize_t read;// nombre de caractère lu par la ligne courante
	char *token;// chaine de caractère pour stocker chaque partie de la ligne (id, processing time, release time)
	char delim=' ';//Délimiteur
	while ((read = getline(&line, &len, file)) != -1) {
		//découpe la ligne en 3 parties,
		token=strtok(line,delim);//id
		char* endptr; // pointeur pour vérifier si la conversion a réussi
		char *id=token;//convertit la chaine de caractère en char *
		token=strtok(NULL,delim);//processing time
		unsigned long processing_time=strtoul(token,&endptr,10);//convertit la chaine de caractère en unsigned long
		if (endptr == token || *endptr != '\0') { // la conversion a échoué
			ShowMessage("erreur processing time ",1);
		}
		token=strtok(NULL,delim);//release time
		unsigned long release_time=strtoul(token,&endptr,10);//convertit la chaine de caractère en unsigned long
		if (endptr == token || *endptr != '\0') { // la conversion a échoué
			ShowMessage("erreur releasedtime",1);
		}
		//crée une nouvelle tâche avec ces 3 parties et l'ajoute à l'instance
		struct task_t * task = new_task(id, processing_time, release_time);//crée une nouvelle tâche
		add_to_list(I, task);//ajoute la tâche à l'instance
	}
	//Libère la mémoire
	free(line);//libère la mémoire allouée par getline
	free(token);//libère la mémoire allouée par strtok
	fclose(file);//ferme le fichier
	return I;//retourne l'instance
}

void view_instance(Instance I) {
	assert(I);//vérifie que l'instance n'est pas null
	view_list(I);//affiche l'instance
}

void delete_instance(Instance I, int deleteData) {
	assert(I);//vérifie que l'instance n'est pas null
	struct list_node_t * node = I->head;//pointeur sur le premier élément de la liste
	while (node) {//parcours la liste
		struct list_node_t * next = node->successor;//pointeur sur le prochain élément de la liste
		if (deleteData){//si deleteData est vrai
			I->freeData(node->data);//supprime les données de la tâche
		}
		free(node);//libère la mémoire allouée pour le noeud
		node = next;//passe au noeud suivant
	}
	free(I);//libère la mémoire allouée pour l'instance
}