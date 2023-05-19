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
	free(i1);
	free(i2);
	free(i3);
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
	free(i1);
	free(i2);
	free(i3);
}
void test_arbre_non_equilibre_rotate_left(){
	struct tree_node_t * racine = new_tree_node(5);
	racine->left = new_tree_node(4);
	racine->left->left = new_tree_node(3);
	racine->left->right = new_tree_node(2);
	racine->right = new_tree_node(6);
	racine = rotate_left(racine->left);//rotation sur le noeud 4
	view_tree(racine);
	//Vérification de l'arbre résultant 
	// Le résultat attendu est :
	//     5
	//    / \	
	//   3   6
	//  / \
	// 2   4

	free_tree(racine);
}

void test_arbre_equilibre_rotate_left(){
	struct tree_node_t * racine = new_tree_node(5);
	racine->left = new_tree_node(3);
	racine->left->left = new_tree_node(2);
	racine->left->right = new_tree_node(4);
	racine->right = new_tree_node(7);
	racine->right->left = new_tree_node(6);
	racine->right->right = new_tree_node(8);
	racine = rotate_left();//rotation sur le noeud 7
	view_tree(racine);

	//Vérification de l'arbre résultant
	// Le résultat attendu est :
	//     5
	//    / \
	//   3   6
	//  / \   \
	// 2   4   7
	//          \
	//           8

	free_tree(racine);
}
void test_arbre_non_equilibre_facteurmoins2_rotate_left(){
	struct tree_node_t * racine = new_tree_node(6);
	racine->left = new_tree_node(5);
	racine->left->left = new_tree_node(4);
	racine->left->right = new_tree_node(3);
	racine->left->right->left = new_tree_node(2);
	racine->left->right->right = new_tree_node(1);
	racine = rotate_left();//rotation sur le noeud 5
	view_tree(racine);

	//Vérification de l'arbre résultant
	// Le résultat attendu est :
	//	   6
	//	  / \
	//   4   7
	//    \   
	//     5
	//      \
	//       3
	//      / \
	//     2   1 
	
	free_tree(racine);     
}

void test_arbre_non_equilibre_rotate_right(){
	struct tree_node_t * racine = new_tree_node(5);
	racine->left = new_tree_node(4);
	racine->left->right = new_tree_node(2);
	racine->right = new_tree_node(6);
	racine = rotate_right(racine->left);//rotation sur le noeud 5
	view_tree(racine);
	//Vérification de l'arbre résultant 
	// Le résultat attendu est :
	//     4
	//      \	
	//       5
	//      / \
	//     2   6

	free_tree(racine);
}

void test_arbre_equilibre_rotate_right(){
	struct tree_node_t * racine = new_tree_node(5);
	racine->left = new_tree_node(3);
	racine->left->left = new_tree_node(2);
	racine->left->right = new_tree_node(4);
	racine->right = new_tree_node(7);
	racine->right->left = new_tree_node(6);
	racine->right->right = new_tree_node(8);
	racine = rotate_right(racine->left);//rotation sur le noeud 3
	view_tree(racine);

	//Vérification de l'arbre résultant
	// Le résultat attendu est :
	//	       3
	//	     /   \
	//      2     5
	//           / \
	//          4   7
	//             / \
	//            6   8
	free_tree(racine);
}

void test_arbre_non_equilibre_facteurmoins2_rotate_right(){
	struct tree_node_t * racine = new_tree_node(6);
	racine->left = new_tree_node(5);
	racine->left->left = new_tree_node(4);
	racine->left->right = new_tree_node(3);
	racine->left->right->left = new_tree_node(2);
	racine->left->right->right = new_tree_node(1);
	racine = rotate_right(racine->left);//rotation sur le noeud 5
	view_tree(racine);

	//Vérification de l'arbre résultant
	// Le résultat attendu est :
	//	   5	
	//	  / \
	//   4   6 
	// 	    /   
	//     3
	//    / \
	//   2   1   

	
	free_tree(racine);     
}


int main() {
	test_list_insert_last();
	test_arbre_non_equilibre_rotate_left();
	test_arbre_equilibre_rotate_left();
	test_arbre_non_equilibre_facteurmoins2_rotate_left();
	test_arbre_non_equilibre_rotate_right();
	test_arbre_equilibre_rotate_right();
	test_arbre_non_equilibre_facteurmoins2_rotate_right();
	return EXIT_SUCCESS;
}