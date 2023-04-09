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

// La fonction SPT calcule une ordonnance de tâches en utilisant l'algorithme Shortest Processing Time First (SPT).
// Elle prend en entrée le nombre de machines ainsi que deux arbres de recherche contenant les tâches prêtes à être exécutées (ready_tasks) et les événements du calendrier (E).
struct schedule_t SPT(int num_m, struct tree_t *ready_tasks, struct tree_t *E) {
    // Création d'une nouvelle structure schedule_t pour stocker l'ordonnance.
    struct schedule_t *S = new_schedule(num_m);
    // Déclaration de variables locales.
    struct event_key_t *event;
    struct task_t *ready_task1, *ready_task2;
    void **key = NULL, **data = NULL;
    // Création d'un nouvel arbre de recherche Q pour stocker les tâches prêtes à être exécutées qui ne peuvent pas encore être assignées à une machine.
    struct tree_t *Q = new_tree(0, intSmallerThan, viewInt, freeInt);
    int i, j;

    // Tant que l'arbre de recherche E n'est pas vide, on exécute le corps de la boucle.

    while (!tree_is_empty(E)) {
        // Trouver l'événement le plus tôt dans E
        struct tree_node_t *minNode = tree_min(get_root(E));
        event = (struct event_key_t *)minNode->key;
        key = (void **)&event->event_time;
        data = (void **)&minNode->data;

        // Retirer l'événement de E
        tree_remove(E, event);

        i = *(int *)*key;
        if (i == 0) {
            // Si l'événement est un départ de tâche prête, 
            // trouver une machine vide pour y placer la tâche
            int M = find_empty_machine(S, i);
            ready_task1 = *(struct task_t **)data;
            if (M >= 0) {
                // Si une machine est disponible, ajouter la tâche à la planification S
                // et ajouter un événement de fin pour cette tâche dans E
                add_task_to_schedule(S, ready_task1, M, i, i + ready_task1->processing_time);
                tree_insert(E, new_event_key(i + ready_task1->processing_time, ready_task1->task_id, 1), NULL);
            } else {
                // Si aucune machine n'est disponible, ajouter la tâche dans Q pour plus tard
                tree_insert(Q, new_ready_task_key(i + ready_task1->processing_time, ready_task1->task_id, ready_task1));
            }
        } else if (i == 1) {
            // Si l'événement est une fin de tâche, ajouter la tâche suivante à la planification S
            ready_task2 = *(struct task_t **)data;
            if (!tree_is_empty(Q)) {
                // Si Q n'est pas vide, prendre la tâche prête la plus tôt
                // et l'ajouter à la planification S avec la machine de la tâche qui vient de finir
                struct tree_node_t *ready_task_node = tree_min(get_root(Q));
                ready_task1 = *(struct task_t **)&ready_task_node->data;
                add_task_to_schedule(S, ready_task1, ready_task2->machine, i, i + ready_task1->processing_time);
                // Ajouter un événement de fin pour la tâche ajoutée à S
                tree_insert(E, new_event_key(i + ready_task1->processing_time, ready_task1->task_id, 0, ready_task2->machine), NULL);
                // Retirer la tâche ajoutée de Q
                tree_remove(Q, ready_task_node);
            } else {
                // Si Q est vide, ajouter simplement la tâche qui vient de finir à la planification S
                j = i + ready_task2->processing_time;
                add_task_to_schedule(S, ready_task2, ready_task2->machine, i, j);
            }
        } 
    }
    // Retourner la planification S complète
    return *S;
}

void SRPT(int num_m, struct tree_t* ready_tasks, struct tree_t* E) {
    struct schedule_t S = new_schedule(num_m);
    struct event_key_t* event;
    struct task_t* ready_task1;
    struct task_t* ready_task2;
    void* key = NULL;
    void* data = NULL;
    int i,j;

    // on parcourt les tâches pour extraire la clef et les données
    while (!tree_is_empty(ready_tasks)) {
        struct tree_node_t* minNode = tree_min(get_root(ready_tasks));
        if (key == NULL) {
            key = minNode->key;
        }
        if (data == NULL) {
            data = minNode->data;
        }
        tree_remove(ready_tasks, get_tree_node_key(minNode));

        ready_task1 = (struct task_t*)data;

        // on insère l'événement de début de tâche dans l'arbre des événements
        tree_insert(E, new_event_key(0, ready_task1->released_time, 0, ready_task1->id, -1), ready_task1);

        // on trouve une machine libre pour exécuter la tâche
        i = find_empty_machine(S, ready_task1->released_time);
        if (i >= 0) { // si une machine est libre
            add_task_to_schedule(S, ready_task1, i, ready_task1->released_time, ready_task1->released_time + ready_task1->processing_time); // on ajoute la tâche à la machine
            tree_insert(E, new_event_key(1, ready_task1->released_time + ready_task1->processing_time, 1, ready_task1->id, i), ready_task1); // on insère l'événement de fin de tâche dans l'arbre des événements
        } else { // sinon, on doit ajouter la tâche dans la file d'attente des tâches prêtes
            tree_insert(ready_tasks, new_ready_task_key(ready_task1->processing_time, ready_task1->id), ready_task1); // on insère la tâche dans l'arbre des tâches prêtes
        }
    }

    // on reproduit le pattern décrit dans le diaporama
    while (!tree_is_empty(E)) {
        struct tree_node_t* minNode = tree_min(get_root(E));
        key = minNode->key;
        data = minNode->data;
        tree_remove(E, get_tree_node_key(minNode));

        event = (struct event_key_t*)key;
        i = event->machine;
        j = event->task_id;

        if (event->event_type == 0) {
        if (get_task(ready_tasks, event->task_id) != NULL) {
            ready_task2 = get_task(ready_tasks, event->task_id);
            int machine_index = find_empty_machine(S, event->event_time);
            if (machine_index != -1) {
                add_task_to_schedule(S, ready_task2, machine_index, event->event_time, event->event_time + ready_task2->processing_time);
                tree_insert(E, new_event_key(1, event->event_time + ready_task2->processing_time, 1, event->task_id, machine_index), ready_task2);
            } else {
                tree_insert(ready_tasks, new_ready_task_key(ready_task2->processing_time, event->task_id), ready_task2);
            }
        } else {
            printf("Error: Task %d not found in ready tasks\n", event->task_id);
        }
    } else if(event->event_type == 1){
        int machine_index = find_empty_machine(S, event->release_time);
        if(machine_index != -1){
            if(!tree_is_empty(ready_tasks)){
                ready_task2 = tree_min(ready_tasks);
                add_task_to_schedule(S, ready_task2, machine_index, event->start_time, event->start_time + ready_task2->processing_time);
                tree_insert(E, new_event_key(1, event->start_time + ready_task2->processing_time, 1, ready_task2->id, machine_index), ready_task2);
                tree_remove(ready_tasks, get_tree_node_key(tree_min(get_root(ready_tasks))));
            }
        }
    }
	}
    delete_event_key(event);
    view_schedule(S);
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
