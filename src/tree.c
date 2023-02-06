#include "tree.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "list.h"
#include "util.h"

/********************************************************************
 * tree_node_t
 ********************************************************************/

/**
 * @brief
 * Construire et initialiser un nouveau nœud d'un arbre binaire de recherche.
 * Le facteur d'équilibre est initialisé à zero.
 * 
 * @param[in] key Clé du nouveau nœud.
 * @param[in] data Donnée à affecter au nouveau nœud.
 * @return struct tree_node_t* Le nouveau nœud créé.
 */
static struct tree_node_t * new_tree_node(void * key, void * data) {
	struct tree_node_t * newTreeNode = malloc(sizeof(struct tree_node_t));
	newTreeNode->key = key;
	newTreeNode->data = data;
	newTreeNode->left = NULL;
	newTreeNode->right = NULL;
	newTreeNode->bfactor = 0;
	return newTreeNode;
}

int tree_node_is_empty(struct tree_node_t * node) {
	if (node == NULL) {
		return 1;
	}
	return 0;
}

void * get_tree_node_key(const struct tree_node_t * node) {
	return node->key;
}

void * get_tree_node_data(const struct tree_node_t * node) {
	return node->data;
}

struct tree_node_t * get_left(const struct tree_node_t * node) {
	return node->left;
}

struct tree_node_t * get_right(const struct tree_node_t * node) {
	return node->right;
}

int get_bfactor(const struct tree_node_t * node) {
	return node->bfactor;
}

void set_tree_node_key(struct tree_node_t * node, void * newKey) {
	node->key = newKey;
}

void set_tree_node_data(struct tree_node_t * node, void * newData) {
	node->data = newData;
}

void set_left(struct tree_node_t * node, struct tree_node_t * newLeft) {
	node->left = newLeft;
}

void set_right(struct tree_node_t * node, struct tree_node_t * newRight) {
	node->right = newRight;
}

void increase_bfactor(struct tree_node_t * node) {
	node->bfactor++;
}

void decrease_bfactor(struct tree_node_t * node) {
	node->bfactor--;
}

void set_bfactor(struct tree_node_t * node, int newBFactor) {
	node->bfactor = newBFactor;
}

/*********************************************************************
 * tree_t
 *********************************************************************/

struct tree_t * new_tree(int balanced, int (*preceed)(const void *, const void *),
							void (*viewKey)(const void *), void (*viewData)(const void *),
							void (*freeKey)(void *), void (*freeData)(void *)) {
	struct tree_t * newTree = malloc(sizeof(struct tree_t));
	newTree->root = NULL;
	newTree->numelm = 0;
	newTree->balanced = balanced;
	newTree->preceed = preceed;
	newTree->viewKey = viewKey;
	newTree->viewData = viewData;
	newTree->freeKey = freeKey;
	newTree->freeData = freeData;
	return newTree;
}

int tree_is_empty(struct tree_t * T) {
	if(T->root == NULL) {
		return 1;
	}
	return 0;
}

int tree_is_balanced(struct tree_t * T) {
	return T->balanced;
}

int get_tree_size(const struct tree_t * T) {
	return T->numelm;
}

struct tree_node_t * get_root(const struct tree_t * T) {
	return T->root;
}

void increase_tree_size(struct tree_t * T) {
	// A FAIRE
}

void decrease_tree_size(struct tree_t * T) {
	// A FAIRE
}

void set_root(struct tree_t * T, struct tree_node_t * newRoot) {
	T->root = newRoot;
}

/**
 * @brief
 * Libérer récursivement le sous-arbre raciné au nœud curr.
 * Dans le cas où le pointeur de fonction freeKey (resp. freeData) n'est pas NULL,
 * la mémoire de la clé (resp. de la donnée) du nœud actuel est aussi libérée.
 * NB : procédure récursive.
 * 
 * @param[in] curr 
 * @param[in] freeKey 
 * @param[in] freeData 
 */
static void delete_tree_node(struct tree_node_t * curr, void (*freeKey)(void *), void (*freeData)(void *)) {
	// A FAIRE
}

/**
 * NB : Utiliser la procédure récursive delete_tree_node.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void delete_tree(struct tree_t * T, int deleteKey, int deleteData) {
	assert(deleteKey == 0 || deleteKey == 1);
	assert(deleteData == 0 || deleteData == 1);
	// A FAIRE
}

/**
 * @brief
 * Afficher récursivement le sous-arbre raciné au nœud curr
 * en utilisant un ordre infixe.
 * NB : procédure récursive.
 *
 * @param[in] curr 
 * @param[in] viewKey 
 * @param[in] viewData 
 */
static void view_tree_inorder(struct tree_node_t * curr,
						void (*viewKey)(const void *),
						void (*viewData)(const void *)) {
	// A FAIRE
}

/**
 * NB : Utiliser la procédure récursive view_tree_inorder.
 */
void view_tree(const struct tree_t * T) {
	// A FAIRE
}

/**
 * @brief
 * Effectuer une rotation gauche autour du nœud \p y.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds
 * modifiés. Pour ce dernier, il y a 5 cas à considérer :
 * (+) bfactor(y)=-2 et bfactor(y->right)=-1
 * (+) bfactor(y)=-2 et bfactor(y->right)=0
 * (+) bfactor(y)=-1 et bfactor(y->right)=1
 * (+) bfactor(y)=-1 et bfactor(y->right)=-1
 * (+) bfactor(y)=-1 et bfactor(y->right)=0
 * (+) bfactor(y)=-2 et bfactor(y->right)=-2
 * 
 * @param[in] y 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * rotate_left(struct tree_node_t * y) {
	assert(y);
	assert(get_right(y));
	// A FAIRE
}

/**
 * @brief
 * Effectuer une rotation droite autour du nœud \p x.
 * N'oubliez pas à mettre à jour les facteurs d'équilibre (bfactor) des nœuds
 * modifiés. Pour ce dernier, il y a 5 cas à considérer :
 * (+) bfactor(x)=2 et bfactor(x->left)=1
 * (+) bfactor(x)=2 et bfactor(x->left)=0
 * (+) bfactor(x)=1 et bfactor(x->left)=1
 * (+) bfactor(x)=1 et bfactor(x->left)=-1
 * (+) bfactor(x)=1 et bfactor(x->left)=0
 * (+) bfactor(x)=2 et bfactor(x->left)=2
 * 
 * @param[in] x 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * rotate_right(struct tree_node_t * x) {
	assert(x);
	assert(get_left(x));
	// A FAIRE
}

/**
 * @brief
 * Insérer un nouveau nœud de clé \p key et donnée \p data
 * au sous-arbre binaire de recherche raciné au nœud \p curr.
 * Les clés sont comparées en utilisant le pointeur de fonction \p preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud \p curr.
 * N'oubliez pas à faire les rotations nécessaires (4 cas à considérer)
 * si le paramètre \p balanced indique que l'arbre est équilibré.
 * La fonction renvoie le nœud curant éventuellement mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[in] data 
 * @param[in] balanced 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * insert_into_tree_node(struct tree_node_t * curr, void * key, void * data,
											int balanced, int (*preceed)(const void *, const void *)) {
	// PARTIE 1 :
	// Mettez ici le code de l'insertion:
	// - exception
	// - recherche récursif de la position à insérer
	// - mise à jour du facteur d'équilibre

	// A FAIRE

	if (balanced) {
		// PARTIE 2 :
		// Gérer ici les rotations

		// A FAIRE
	}

	return ;
}

/**
 * NB : Utiliser la fonction récursive insert_into_tree_node.
 */
void tree_insert(struct tree_t * T, void * key, void * data) {
	// A FAIRE
}

struct tree_node_t * tree_min(struct tree_node_t * curr) {
	assert(!tree_node_is_empty(curr));
	// A FAIRE
}

struct tree_node_t * tree_max(struct tree_node_t * curr) {
	assert(!tree_node_is_empty(curr));
	// A FAIRE
}

struct tree_node_t * tree_find_node(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr));
	// A FAIRE
}

struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr));
	// A FAIRE
}

struct tree_node_t * tree_find_successor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr));
	// A FAIRE
}

/**
 * @brief 
 * Supprimer le nœud de clé \p key du sous-arbre binaire de recherche raciné au nœud \p curr.
 * La donnée du nœud supprimé est restituée en utilisant le pointeur \p data (passage de paramètre par référence).
 * Les clés sont comparées en utilisant le pointeur de fonction \p preceed.
 * N'oubliez pas à mettre à jour la facteur d'équilibre du nœud \p curr.
 * N'oubliez pas à faire les rotations nécessaires (6 cas à considérer)
 * si le paramètre \p balanced indique que l'arbre est équilibré.
 * La fonction renvoie le nœud curant éventuellement mis à jour.
 * NB : fonction récursive.
 * 
 * @param[in] curr 
 * @param[in] key 
 * @param[out] data 
 * @param[in] balanced 
 * @param[in] preceed 
 * @return struct tree_node_t* 
 */
static struct tree_node_t * remove_tree_node(struct tree_node_t * curr, void * key, void ** data, int balanced, 
											int (*preceed)(const void *, const void *)) {
	assert(curr);
	// PARTIE 1 :
	// Mettez ici le code de la suppression:
	// - exception
	// - recherche récursif de la clé à supprimer
	// - mise à jour du facteur d'équilibre

	// A FAIRE

	if (balanced && curr) {
		// PARTIE 2 :
		// Gérer ici les rotations

		// A FAIRE
	}

	return ;
}

/**
 * NB : Utiliser la fonction récursive remove_tree_node.
 */
void * tree_remove(struct tree_t * T, void * key) {
	// A FAIRE
}
