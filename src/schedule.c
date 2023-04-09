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
	assert(bt < et);//begin time doit être inférieur à end time
	if (task == NULL) {//si la tâche est nulle
		return NULL;//on renvoie NULL
	}
	struct schedule_node_t * snode = (struct schedule_node_t *) malloc(sizeof(struct schedule_node_t));//on alloue de la mémoire pour le nœud
	if (snode == NULL) {//si l'allocation a échoué
		return NULL;//on renvoie NULL
	}
	snode->task = task;//on initialise task du nœud
	snode->begin_time = bt;//on initialise begin_time du nœud
	snode->end_time = et;//on initialise end_time du nœud
	return snode;//on renvoie le nœud
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
	printf("Task %d \n, begin time %lu\n, end time %lu\n)", get_task_id(get_schedule_node_task(snode)), get_schedule_node_begin_time(snode), get_schedule_node_end_time(snode));//on affiche les informations du nœud
}

void delete_schedule_node(void * snode) {
	if(snode != NULL) {//si le nœud n'est pas nul
		free(snode);//on libère la mémoire
	}
}

/********************************************************************
 * schedule_t
 ********************************************************************/

struct schedule_t * new_schedule(int num_m) {
    assert(num_m >= 1);

    struct schedule_t * S = (struct schedule_t *) malloc(sizeof(struct schedule_t));
    if (!S) {
        return NULL;
    }

    S->num_machines = num_m;
    S->machines = (struct list_t **) malloc(num_m * sizeof(struct list_t *));
    if (!S->machines) {
        free(S);
        return NULL;
    }

    for(int i = 0; i < num_m; i++){
        S->machines[i] = new_list(view_schedule_node, delete_schedule_node);
        if (!S->machines[i]) {
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
	assert(machine >= 0 && machine < S->num_machines);//on vérifie que la machine est bien dans l'ordonnancement
	return S->machines[machine];//on renvoie la liste de la machine
}

int get_num_machines(const struct schedule_t * S) {
	return S->num_machines;//on renvoie le nombre de machines
}

void view_schedule(const struct schedule_t * S) {
    if (S == NULL) {
        return;
    }
    fprintf(stdout, "Schedule: %d machines", S->num_machines);
    for (int i = 0; i < S->num_machines; i++) {
        fprintf(stdout, "Machine %d\n", i);
        for (struct list_node_t * temp = get_list_head(get_schedule_of_machine(S,i)); temp != NULL; temp = temp->successor) {
            view_task(get_schedule_node_task(get_list_node_data(temp)));
            fprintf(stdout, "\n");
        }
    }
}

void delete_schedule(struct schedule_t * S) {
	for(int i = 0; i < S->num_machines; i++){//on parcourt toutes les machines
		delete_list(S->machines[i]);//on libère la mémoire de la liste de la machine i
	}
	free(S);//on libère la mémoire
}

// Pour le format du fichier à créer, regardez dans la présentation du cours.
void save_schedule(struct schedule_t * S, char * filename) {
	assert(filename == NULL);//on vérifie que le nom du fichier n'est pas nul
	FILE * file = fopen(filename, "w");//on ouvre le fichier en écriture
	if(file == NULL){//si l'ouverture a échoué
		ShowMessage("Fichier impossible à ouvrir",1);//on affiche un message d'erreur
	}
	fprintf(file, "%d\n", S->num_machines);//on écrit le nombre de machines dans le fichier
	for(int i = 0; i < S->num_machines; i++){//on parcourt toutes les machines
		struct list_t * list = S->machines[i];//on récupère la liste de la machine i
		struct schedule_node_t * snode = get_list_head(list);//on récupère le premier nœud de la liste
			while(snode != NULL){//tant que le nœud n'est pas null
				fprintf(file, "%s %lu %lu\n", get_task_id(get_schedule_node_task(snode)), get_schedule_node_begin_time(snode), get_schedule_node_end_time(snode));//on écrit les informations du nœud dans le fichier
				snode = get_list_next(list, snode);//on passe au nœud suivant
			}
		}
		fclose(file);//on ferme le fichier
}

// Trouver la machine qui est vide
int find_empty_machine(struct schedule_t * S, unsigned long time) {
	assert (S == NULL);//on vérifie que l'ordonnancement n'est pas null
	assert (time > 0);//on vérifie que l'ordonnancement et le temps sont valides
	for(int i = 0; i < S->num_machines; i++){// on parcourt toutes les machines
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
	assert(S == NULL);//on vérifie que l'ordonnancement n'est pas nul
	assert(time > 0);//on vérifie que l'ordonnancement et le temps sont valides
	assert(processing_time > 0);//on vérifie que l'ordonnancement et le temps de traitement sont valides
	int stop = -1;//on initialise la variable stop à -1
	for(int i = 0; i < S->num_machines; i++){// on parcourt toutes les machines
		struct list_t * list = S->schedule[i];//on récupère la liste de la machine i
		if(list != NULL){// si la liste n'est pas vide
			struct list_node_t * node = get_list_tail(list);//on récupère le dernier élément de la liste
			struct task_t * task = get_list_node_data(node);//on récupère la tâche de la machine i
			if(get_task_duration(task) + get_schedule_node_begin_time(node) <= time){//si la durée de la tâche + le temps de début est inférieur au temps
				if(stop == -1){//si stop est égal à -1
					stop = i;//on affecte à stop la machine i
				}
				else if(get_schedule_node_end_time(get_list_node_data(S->schedule[stop])) > get_schedule_node_end_time(get_list_node_data(S->schedule[i]))){//sinon si le temps de fin de la tâche de la machine stop est supérieur au temps de fin de la tâche de la machine i
					stop = i;//on affecte à stop la machine i
				}else{
					stop = stop;//sinon on ne fait rien
				}
			}
		}
	}
	return stop;//on retourne stop
}

void add_task_to_schedule(struct schedule_t * S, struct task_t * task, int machine, unsigned long bt, unsigned long et) {
	assert(machine >= 0 || machine < S->num_machines);//on vérifie que la machine est valide
	assert(bt < et);//on vérifie que le temps de début est inférieur au temps de fin
	list_insert_last(S->machines[machine], new_schedule_node(task, bt, et));//on ajoute la tache en fin de liste de la machine
}

unsigned long preempt_task(struct schedule_t * S, int machine, unsigned long new_et) {
	assert(machine >= 0 || machine < S->num_machines);//on vérifie que la machine est valide
	struct list_t * list = S->schedule[machine];//on récupère la liste de la machine
	struct list_node_t * node = get_list_tail(list);//on récupère le dernier élément de la liste
	struct task_t * task = get_list_node_data(node);//on récupère la tâche de la machine
	assert(task == NULL);//on vérifie que la tâche n'est pas null
	unsigned long old_et = task->release_time + task->processing_time;//on récupère le temps de fin de la tâche
	task->processing_time = new_et - task->release_time;//on modifie le temps de traitement de la tâche
	S->machines[machine]->tail->end_time = new_et;//on modifie le temps de fin de la tâche
	return new_et;
}

unsigned long get_makespan(struct schedule_t * S) {
	assert(S == NULL);//on vérifie que l'ordonnancement n'est pas null
	unsigned long makespan = 0;//on initialise la variable makespan à 0
	for(int i = 0; i < S->num_machines; i++){//on parcourt toutes les machines
		struct list_t * list = S->machines[i];//on récupère la liste de la machine i
		struct schedule_node_t * snode = get_list_tail(list);//on récupère le dernier élément de la liste
		if(snode != NULL){//si la liste n'est pas vide
			unsigned long end_time = get_schedule_node_end_time(get_list_node_data(snode));
			if(end_time > makespan){//si le temps de fin de la tâche est supérieur au makespan
				makespan = end_time;//on affecte à makespan le temps de fin de la tâche
			}
		}
	}
	return makespan;//on retourne le makespan
}