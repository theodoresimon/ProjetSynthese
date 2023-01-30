#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "util.h"
#include "tree.h"

/********************************************************************
 * list_node_t
 ********************************************************************/

/**
 * @brief
 * Construit et initialise un nouveau nœud d'une liste doublement chaînée.
 * 
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return struct list_node_t* Le nouveau nœud créé.
 */
static struct list_node_t * new_list_node(void * data) {
	struct list_node_t * newListNode = malloc(sizeof(struct list_node_t));
	newListNode->data = data;
	newListNode->successor = NULL;
	newListNode->predecessor = NULL;
}

void * get_list_node_data(const struct list_node_t * node) {
	assert(node);
	return node->data;
}

struct list_node_t * get_successor(const struct list_node_t* node) {
	assert(node);
	return node->successor;
}

struct list_node_t * get_predecessor(const struct list_node_t * node) {
	// A FAIRE
}

void set_list_node_data(struct list_node_t * node, void * newData) {
	// A FAIRE
}

void set_successor(struct list_node_t * node, struct list_node_t * newSucc) {
	// A FAIRE
}

void set_predecessor(struct list_node_t * node, struct list_node_t * newPred) {
	// A FAIRE
}

/********************************************************************
 * list_t
 ********************************************************************/

struct list_t * new_list(void (*viewData)(const void*), void (*freeData)(void*)) {
	// A FAIRE
}

int list_is_empty(struct list_t * L) {
	// A FAIRE
}

int get_list_size(const struct list_t * L) {
	// A FAIRE
}

struct list_node_t * get_list_head(const struct list_t * L) {
	// A FAIRE
}

struct list_node_t * get_list_tail(const struct list_t * L) {
	// A FAIRE
}

void increase_list_size(struct list_t * L) {
	// A FAIRE
}

void decrease_list_size(struct list_t * L) {
	// A FAIRE
}

void set_list_size(struct list_t * L, int newSize) {
	// A FAIRE
}

void set_head(struct list_t * L, struct list_node_t * newHead) {
	// A FAIRE
}

void set_tail(struct list_t * L, struct list_node_t * newTail) {
	// A FAIRE
}

void delete_list(struct list_t * L, int deleteData) {
	// A FAIRE
}

void view_list(const struct list_t * L) {
	// A FAIRE
}

void list_insert_first(struct list_t * L, void * data) {
	// A FAIRE
}

void list_insert_last(struct list_t * L, void * data) {
	// A FAIRE
}

void list_insert_after(struct list_t * L, void * data, struct list_node_t * ptrelm) {
	// A FAIRE
}

void * list_remove_first(struct list_t * L) {
	assert(get_list_head(L));
	// A FAIRE
}

void * list_remove_last(struct list_t * L) {
	assert(get_list_head(L));
	// A FAIRE
}

void * list_remove_node(struct list_t * L, struct list_node_t * node) {
	assert(get_list_head(L) && get_list_tail(L));
	// A FAIRE
}