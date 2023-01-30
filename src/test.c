#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "util.h"
#include "list.h"
#include "tree.h"
#include "instance.h"
#include "schedule.h"
#include "algo.h"

static int compare_lists(struct list_t *l1, double* l2[], int size) {
	if (get_list_size(l1) != size)
		return 0;

	if (get_list_head(l1) == NULL)
		return 1;

	struct list_node_t * curr = get_list_head(l1);
	int i = 0;
	while (curr != NULL) {
		if (get_list_node_data(curr) != l2[i])
			return 0;
		curr = get_successor(curr);
		i++;
	}

	curr = get_list_tail(l1);
	i = size-1;
	while (curr != NULL) {
		if (get_list_node_data(curr) != l2[i])
			return 0;
		curr = get_predecessor(curr);
		i--;
	}
	return 1;
}

void test_list_insert_last() {
	int *i1 = malloc(sizeof(int));
	int *i2 = malloc(sizeof(int));
	int *i3 = malloc(sizeof(int));
	*i1 = 1;
	*i2 = 2;
	*i3 = 3;

	struct list_t * L = new_list(viewInt, freeInt);
	int* tab[3];
	tab[0] = i1; tab[1] = i2; tab[2] = i3;

	list_insert_last(L, i1);
	if (compare_lists(L, tab, 1) == 0) printf("problème");
	list_insert_last(L, i2);
	if (compare_lists(L, tab, 2) == 0) printf("problème");
	list_insert_last(L, i3);
	if (compare_lists(L, tab, 3) == 0) printf("problème");

	// Il manque la libération de la mémoire. C'est à vous !
}

int main() {
	test_list_insert_last();

	return EXIT_SUCCESS;
}