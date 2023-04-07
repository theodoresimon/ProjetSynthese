#include "algo.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <limits.h>

#include "util.h"
#include "list.h"
#include "tree.h"
#include "instance.h"
#include "schedule.h"

/********************************************************************
 * event_key_t
 ********************************************************************/

/**
 * @brief 
 * Une structure qui definit la clé de l'ensemble des événements.
 */
struct event_key_t {
	int event_type; // 0 si l'événement correspond à la libération d'une tâche,
					// 1 si l'événement correspond à la fin d'exécution d'une tâche
	unsigned long event_time;
	unsigned long processing_time; // par convention, 0 si l'événement correspond à la fin d'exécution d'une tâche
								   // sinon, la durée opératoire de la tâche qui va être libérée
	char * task_id;
	int machine; // par convention, 0 si l'événement correspond à la libération d'une tâche,
				 // sinon, la machine qui exécute la tâche de l'événement
};

/**
 * @brief 
 * 
 * @param[in] event_type 
 * @param[in] event_time 
 * @param[in] processing_time 
 * @param[in] task_id 
 * @param[in] machine 
 * @return struct event_key_t* 
 */
struct event_key_t * new_event_key(int event_type, unsigned long event_time, unsigned long processing_time, char * task_id, int machine) {
	struct event_key_t * key = (struct event_key_t *) malloc(sizeof(struct event_key_t));//on alloue de la mémoire pour la clé
	key->event_type = event_type;//on initialise event_type de la clé
	key->event_time = event_time;//on initialise event_time de la clé
	key->processing_time = processing_time;//on initialise processing_time de la clé
	key->task_id = task_id;//on initialise task_id de la clé
	key->machine = machine;//on initialise machine de la clé
	return key;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return int 
 */
int get_event_type(const struct event_key_t * key) {
	return key->event_type;//on retourne event_type de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_event_time(const struct event_key_t * key) {
	return key->event_time;//on retourne event_time de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_event_processing_time(const struct event_key_t * key) {
	return key->processing_time;//on retourne processing_time de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return char* 
 */
char * get_event_task_id(const struct event_key_t * key) {
	return key->task_id;//on retourne task_id de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return int 
 */
int get_event_machine(const struct event_key_t * key) {
	return key->machine;//on retourne machine de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void view_event_key(const void * key) {
	printf("event_type=%d, event_time=%lu, processing_time=%lu, task_id=%s, machine=%d", get_event_type(key), get_event_time(key), get_event_processing_time(key), get_event_task_id(key), get_event_machine(key));//on affiche les valeurs de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void delete_event_key(void * key) {
	free(key);//on libère la mémoire de la clé
}

/**
 * @brief Comparer deux événements \p a et \p b.
 * Les règles de comparaison sont données dans la présentation du cours.
 * 
 * @param[in] a 
 * @param[in] b 
 * @return int 
 */
int event_preceed(const void * a, const void * b) {
	return (get_event_time(a) < get_event_time(b)) ||(get_event_time(a) == get_event_time(b) && get_event_processing_time(a)< get_event_processing_time(b));//on compare les valeurs de la clé
}

/********************************************************************
 * ready_task_key_t
 ********************************************************************/

/**
 * @brief
 * Une structure qui définit la clé de la file d'attente avec les "ready tasks".
 */
struct ready_task_key_t {
	unsigned long remaining_processing_time;
	char * task_id;
};

/**
 * @brief 
 * 
 * @param[in] remaining_processing_time 
 * @param[in] task_id 
 * @return struct ready_task_key_t* 
 */
struct ready_task_key_t * new_ready_task_key(unsigned long remaining_processing_time, char * task_id) {
	struct ready_task_key_t * key = (struct ready_task_key_t *) malloc(sizeof(struct ready_task_key_t));//on alloue de la mémoire pour la clé
	key->remaining_processing_time = remaining_processing_time;//on initialise remaining_processing_time de la clé
	key->task_id = task_id;//on initialise task_id de la clé
	return key;
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return unsigned long 
 */
unsigned long get_ready_task_remaining_processing_time(const struct ready_task_key_t * key) {
	return key->remaining_processing_time;//on retourne remaining_processing_time de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 * @return char* 
 */
char * get_ready_task_id(const struct ready_task_key_t * key) {
	return key->task_id;//on retourne task_id de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void view_ready_task_key(const void * key) {
	printf("remaining_processing_time=%lu, task_id=%s", get_ready_task_remaining_processing_time(key), get_ready_task_id(key));//on affiche les valeurs de la clé
}

/**
 * @brief 
 * 
 * @param[in] key 
 */
void delete_ready_task_key(void * key) {
	free(key);//on libère la mémoire de la clé
}

/**
 * @brief Comparer deux tâches \p a et \p b de la file d'attente afin de définir la tâche la plus prioritaire.
 * Les deux paramètres d'entrée \p a et \p b sont de type struct ready_task_key_t *.
 * Les règles de comparaison sont données dans la présentation du cours.
 *
 * @param[in] a 
 * @param[in] b 
 * @return int 
 */
int ready_task_preceed(const void * a, const void * b) {
	return (get_ready_task_remaining_processing_time(a) < get_ready_task_remaining_processing_time(b));//on compare les valeurs de la clé
}

/********************************************************************
 * The algorithm
 ********************************************************************/

struct schedule_t SPT(int num_m, struct tree_t * ready_tasks, struct tree_t * E){
	struct schedule_t * S = new_schedule(num_m);//on crée un nouveau schedule
	struct event_key_t * event;//on crée un nouvel événement
	struct task_t * ready_task1;//on crée une nouvelle tâche prête
	struct task_t * ready_task2;//on crée une nouvelle tâche prête
	void ** key;//on crée une clé
	void ** data;//on crée des données	
	int i;
		//on parcourt les tâches pour extraire la clef et les données
		struct tree_node_t* minNode = tree_min(get_root(E));//on récupère le noeud minimum de l'arbre
		if(key != NULL){
			*key = minNode->key;//on récupère la clé du noeud minimum
		}
		if(data != NULL){
			*data = minNode->data;//on récupère les données du noeud minimum
		}
		tree_remove(E, get_tree_node_key(minNode));//on supprime le noeud minimum de l'arbre
		//on reproduit le pattern décrit dans le diaporama
	while(!tree_is_empty(E)){//tant que l'arbre n'est pas vide
		i = key->event_time;//on récupère le temps de l'événement

		// si i concerne la libération d'une tâche
		if(i== 0){
			//si il existe une machine libre en i
			int M = find_empty_machine(S, i);
			if(M >= 0){//si la machine est libre
				add_task_to_schedule(S,ready_task1,M,i,i+ready_task1->processing_time);//on ajoute la tâche à la machine
				tree_insert(E,new_event_key(i+ready_task1->processing_time,ready_task1->task_id),ready_task1);//on insère l'événement dans l'arbre
			}
			else{
				//sinon il faut le mettre dans la file d'attente
				tree_insert(Q,new_ready_task_key(Q,new_ready_task_key(i+ready_task1->processing_time,ready_task1->task_id),ready_task1));//on insère la tâche dans la file d'attente
			}
		}else if(i == 1){//si i concerne la fin d'une tâche
			//si Q n'est pas vide
			if(!tree_is_empty(Q)){
				//on extrait la tâche de Q
				ready_task2 = tree_min(Q);
				add_task_to_schedule(S,ready_task2->processing_time,e->machine,i,i+ready_task2->processing_time);//on ajoute la tâche à la machine
				tree_insert(E,new_event_key(1,i+ready_task2->processing_time,0,ready_task2->id,e->machine),ready_task2);//on insère l'événement dans l'arbre
			}
		}
		delete_event_key(event);//on libère la mémoire de l'événement
	}
	return S;
}

struct schedule_t SRPT(int num_m, struct tree_t * ready_tasks, struct tree_t * E){
	struct schedule_t * S = new_schedule(num_m);//on crée un nouveau schedule
	struct event_key_t * event;//on crée un nouvel événement
	struct task_t * ready_task1;//on crée une nouvelle tâche prête
	struct task_t * ready_task2;//on crée une nouvelle tâche prête
	void ** key;//on crée une clé
	void ** data;//on crée des données	
	int i;
	int j;//permetra de stocker le temps de fin de la tâche
		//on parcourt les tâches pour extraire la clef et les données
		struct tree_node_t* minNode = tree_min(get_root(E));//on récupère le noeud minimum de l'arbre
		if(key != NULL){
			*key = minNode->key;//on récupère la clé du noeud minimum
		}
		if(data != NULL){
			*data = minNode->data;//on récupère les données du noeud minimum
		}
		tree_remove(E, get_tree_node_key(minNode));//on supprime le noeud minimum de l'arbre
		//on reproduit le pattern décrit dans le diaporama
	while(!tree_is_empty(E)){//tant que l'arbre n'est pas vide
		i = key->event_time;//on récupère le temps de l'événement

		// si i concerne la libération d'une tâche
		if(i == 0){
			//si il existe une machine libre en i
			int M = find_empty_machine(S, i);
			if(M >= 0){//si la machine est libre
				add_task_to_schedule(S,ready_task1,M,i,i+ready_task->processing_time);//on ajoute la tâche à la machine
				tree_insert(E,new_event_key(0,i+ready_task1->processing_time,0,ready_task->id,M),ready_task1);//on insère l'événement dans l'arbre
			}
			M = find_machine_to_interrupt(S,i,ready_task->processing_time);//on trouve la machine à interrompre
			if(M != -1){//si la machine est libre
				//on preempte la tâche
				j = preempt_task(S,M,i);//on récupère le temps de fin de la tâche
				update_event_time(S,M,i);//on met à jour l'événement de fin de tâche
				//on supprime l'événement de fin de tâche
				struct list_t *L = get_list(S,M);//on récupère la liste de la machine
				struct list_node_t *N = get_list_tail(L);//on récupère le noeud de la liste
				ready_task2=get_list_node_data(N);//on récupère les données du noeud
				struct event_key_t * event = new_event_key(0,i+ready_task2->processing_time,0,ready_task2->id,M);//on crée un nouvel événement
				tree_remove(E,event);//on supprime l'événement de l'arbre)
				tree_insert(Q,new_ready_task_key(ready_task2->processing_time,ready_task2->id),ready_task2);//on insère l'événement dans l'arbre
				add_task_to_schedule(S,ready_task1,M,i,i+ready_task1->processing_time);//on ajoute la tâche à la machine	
				tree_insert(E,new_event_key(0,i+ready_task1->processing_time,0,ready_task1->id,M),ready_task1);//on insère l'événement dans l'arbre
			}
			else{
			//sinon il faut le mettre dans la file d'attente
			tree_insert(Q,new_ready_task_key(ready_task1->processing_time,ready_task1->id),ready_task1);//on insère la tâche dans la file d'attente
			}
		}else if(i == 0){//si i concerne la fin d'une tâche
			//si Q n'est pas vide
			if(!tree_is_empty(Q)){
				//on extrait la tâche de Q
				ready_task2 = tree_min(Q);
				add_task_to_schedule(S,ready_task2->processing_time,e->machine,i,i+ready_task2->processing_time);//on ajoute la tâche à la machine
				tree_insert(E,new_event_key(0,i+ready_task2->processing_time,0,ready_task2->id,e->machine),ready_task2);//on insère l'événement dans l'arbre
			}
		}	
		delete_event_key(event);//on libère la mémoire de l'événement
	}
	return S;//on retourne le schedule
}

struct schedule_t * create_schedule(Instance I, int num_m, int preemption, int balanced_tree) {
	struct tree_t * ready_tasks = new_tree(balanced_tree,ready_task_preceed,view_ready_task_key,view_task,delete_ready_task_key,delete_task);//on crée un nouvel arbre
	struct tree_t * E = new_tree(balanced_tree,event_preceed,view_event_key,view_task,delete_event_key,delete_task);//on crée un nouvel arbre
	for(struct list_node_t * N = get_list_head(I); N != NULL; N = get_list_node_next(N)){//on parcourt les tâches
		struct task_t * task =(struct task_t *) get_list_node_data(N);//on récupère les données du noeud
		struct event_key_t * event = new_event_key(0,task->release_time,task->processing_time,task->id,0);//on crée un nouvel événement
		tree_insert(E,event,task);//on insère l'événement dans l'arbre
	}
	struct schedule_t * S = SRPT(num_m,ready_tasks,E);//on crée un nouveau schedule
	delete_tree(ready_tasks,1,1);//on libère la mémoire de l'arbre
	delete_tree(E,1,1);//on libère la mémoire de l'arbre
	return S;//on retourne le schedule
}
