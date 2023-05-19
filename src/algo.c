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


struct schedule_t * create_schedule(Instance I, int num_m, int preemption, int balanced_tree) {
   struct schedule_t *S = new_schedule(num_m);
    
    //Arbre qui contient les ready_task en file d'attente Q 
    struct tree_t *Q = new_tree(balanced_tree, ready_task_preceed, view_ready_task_key, view_task, delete_ready_task_key, delete_task);
    
    //Arbre qui contient l'ensemble des evenements 
    struct tree_t *E = new_tree(balanced_tree,event_preceed,view_event_key,view_task,delete_event_key,delete_task);
    
    //Ajouter les dates de libération de toutes les tâches dans l'ensemble des événements
    for(struct list_node_t *curr = get_list_head(I); curr != NULL ; curr = get_successor(curr)) {
        struct task_t *task = get_list_node_data(curr);
        struct event_key_t *ev = new_event_key(0, get_task_release_time(task), get_task_processing_time(task), get_task_id(task), 0);
        tree_insert(E, ev, task);
    }
    while(!tree_is_empty(E)) {
        struct tree_node_t *eventMin = tree_min(get_root(E));
        if(get_event_type(get_tree_node_key(eventMin)) == 0) { // Si l'événement est la libération d'une tâche Tj
            int emptyMachine = find_empty_machine(S,get_event_time(get_tree_node_key(eventMin)));
            if(emptyMachine != -1) { // Si une machine est libre à l'instant de libération de la tâche Tj
                struct task_t *task = (struct task_t *) get_tree_node_data(eventMin);
                add_task_to_schedule(S, task, emptyMachine, get_event_time(get_tree_node_key(eventMin)), get_event_time(get_tree_node_key(eventMin)) + get_event_processing_time(get_tree_node_key(eventMin)));
                
                // Ajouter l'événement "fin d'exécution" de la tâche Tj dans l'ensemble des événements
                struct event_key_t *endEvent = new_event_key(1, get_task_release_time(task) + get_task_processing_time(task), 0, get_task_id(task), emptyMachine);
                tree_insert(E,endEvent,task);
            } else if (preemption) { // Si toutes les machines sont occupées et on est en mode préemptif
                int interMachine = find_machine_to_interrupt(S, get_event_time(eventMin->key), get_event_processing_time(eventMin));
                if(interMachine != -1 ) {
                    // Préempter la tâche en cours d'exécution sur la machine interMachine(//On prempt la tache)
                    unsigned long preempt = preempt_task(S, interMachine, get_event_time(get_tree_node_key(eventMin)));

                    //On supprime l'evenement de fin de E 
                    struct schedule_node_t * sNode = get_list_node_data(get_list_tail(get_schedule_of_machine(S,interMachine)));
                    struct task_t * tasK = get_schedule_node_task(sNode);
                    //On va recréé la cle de l'evenement à supprimer
                    struct event_key_t *eventKeyRemv = new_event_key(1,get_schedule_node_end_time(sNode)+preempt,0,get_task_id(tasK),interMachine);
                    //On supprime le noued 
                    tasK = tree_remove(E,eventKeyRemv);
                    delete_event_key(eventKeyRemv);
                    
                    // Ajouter la tâche interrompue dans la file d'attente Q
                    struct ready_task_key_t *intr_key = new_ready_task_key(preempt, get_task_id(tasK));
                    tree_insert(Q, intr_key, tasK);
                    // Ajouter l'événement "fin d'exécution" de la tâche Tj dans l'ensemble des événements
                    struct event_key_t *endEvent2 = new_event_key(1, get_event_time(get_tree_node_key(eventMin)) + get_event_processing_time(get_tree_node_key(eventMin)), 0, get_task_id(get_tree_node_data(eventMin)), interMachine);
                    tree_insert(E,endEvent2,get_tree_node_data(eventMin));
                    // Affecter la tâche Tj à la machine interMachine
                    add_task_to_schedule(S,get_tree_node_data(eventMin),interMachine,get_event_time(get_tree_node_key(eventMin)),get_event_time(get_tree_node_key(eventMin))+get_event_processing_time(get_tree_node_key(eventMin)));
                } else { // On est en mode préémptif, mais on n'a pas pu faire de préemption
                    
                    struct ready_task_key_t *in_key = new_ready_task_key(get_task_processing_time(get_tree_node_data(eventMin)),get_task_id(get_tree_node_data(eventMin)));
                    tree_insert(Q,in_key ,get_tree_node_data(eventMin));
                }
            } else { // Si toutes les machines sont occupées et on n'est pas en mode préemptif, ajouter Tj dans la file d'attente Q
                struct ready_task_key_t *rt_key = new_ready_task_key(get_task_processing_time(get_tree_node_data(eventMin)), get_task_id(get_tree_node_data(eventMin)));
                tree_insert(Q, rt_key, get_tree_node_data(eventMin));
            }
        } else { // Si l'événement est la fin d'exécution d'une tâche Tj
            if(!tree_is_empty(Q)) { // Si la file d'attente Q n'est pas vide
                // Extraire de Q la tâche Tk avec la durée la plus courte (règle SPT)
                struct tree_node_t * rt_key = tree_min(get_root(Q));
                // Affecter Tk à la machine qui vient de libérer Mi
                add_task_to_schedule(S,get_tree_node_data(rt_key),get_event_machine(get_tree_node_key(eventMin)),get_event_time(get_tree_node_key(eventMin)),get_event_processing_time(get_tree_node_key(eventMin)));
                // Ajouter l'événement "fin d'exécution" de la tâche Tk dans l'ensemble des événements
                struct event_key_t *endEvent = new_event_key(1, get_event_time(get_tree_node_key(eventMin)) + get_ready_task_remaining_processing_time(get_tree_node_key(rt_key)), 0, get_task_id(get_tree_node_data(rt_key)),get_event_machine(get_tree_node_key(eventMin)));
                tree_insert(E,endEvent,get_tree_node_data(rt_key));
                struct task_t *task = (struct task_t *)tree_remove(Q,get_tree_node_data(rt_key));
            }
        }
        // Supprimer l'événement de la fin d'exécution de Tj de l'ensemble des événements
        struct task_t *taskk = tree_remove(E, get_tree_node_key(eventMin));
    }
    delete_tree(Q,1,1);
    delete_tree(E,1,1);
    return S;
}
