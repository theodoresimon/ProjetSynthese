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
	return (node == NULL);
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
	return (T->numelm == 0);
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
	T->numelm++;
}

void decrease_tree_size(struct tree_t * T) {
	T->numelm--;
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
	if (curr == NULL) {
		ShowMessage("probleme avec la fontion void delete_tree_node le noeud courant est à Null", 1);
	}
	delete_tree_node(curr->left, freeKey, freeData);//on supprime récursivement tous les noeuds du fils gauche de la racine donc supprimera les feuilles en premier puis remontera  
	delete_tree_node(curr->right, freeKey, freeData);//de même pour les noeuds de droites

	if (freeKey != NULL) {
		freeKey(curr->key);// si freekey n'est pas null alors on supprime la cle du noeud courant 
	}
	if (freeData != NULL) {
		freeData(curr->data);//cas similaire pour freedata que freekey
	}
	free(curr);//on libère le noeud curr
}


/**
 * NB : Utiliser la procédure récursive delete_tree_node.
 * Vous devez utiliser les bons paramètres freeKey et freeData
 * par rapport aux valeurs deleteKey et deleteData.
 */
void delete_tree(struct tree_t * T, int deleteKey, int deleteData) {
	assert(T != NULL);//on vérifie que l'arbre n'est pas vide
	delete_tree_node(T->root,(deleteKey==1) ? T->freeKey : NULL, (deleteData==1) ? T->freeData : NULL);//on supprime l'arbre en utilisant la fonction delete_tree_node et vérifiant si on doit supprimer la cle et la donnée
	free(T);//on libère l'arbre
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
	if (curr != NULL) {
		view_tree_inorder(curr->left, viewKey, viewData);//on affiche récursivement tous les noeuds du fils gauche de la racine donc affichera les feuilles en premier puis remontera
		if (viewKey != NULL) {//si viewkey n'est pas null alors on affiche la cle du noeud courant
			viewKey(curr->key);//on affiche la cle du noeud courant
			printf("\n");//on saute une ligne pour la lisibilité
		}
		if (viewData != NULL) {//cas similaire pour viewdata que viewkey
			viewData(curr->data);//on affiche la donnée du noeud courant
			printf("\n");//on saute une ligne pour la lisibilité
		}
		view_tree_inorder(curr->right, viewKey, viewData);//de même pour les noeuds de droites
	}
}

/**
 * NB : Utiliser la procédure récursive view_tree_inorder.
 */
void view_tree(const struct tree_t * T) {
	view_tree_inorder(T->root, T->viewKey, T->viewData);//on affiche l'arbre en utilisant la fonction view_tree_inorder
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
static struct tree_node_t *rotate_left(struct tree_node_t *y)
{
	assert(y);//on vérifie que le noeud n'est pas null
    assert(get_right(y));//on vérifie que le fils droit du noeud n'est pas null

    struct tree_node_t *x = y->right;//on récupère le fils droit du noeud courant
    struct tree_node_t *T2 = x->left;//on récupère le fils gauche du fils droit du noeud courant

    x->left = y;//on met le fils gauche du fils droit du noeud courant comme fils droit du noeud courant
    y->right= T2;//on met le noeud courant comme fils gauche du fils droit du noeud courant
	//en fonction du facteur d'équilibre du noeud courant et de son fils droit on met à jour les facteurs d'équilibre des noeuds modifiés
    if((y->bfactor == -2)&&(x->bfactor == -1)){
		y->bfactor = 0;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 0;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	else if((y->bfactor == -2)&&(x->bfactor == 0)){
		y->bfactor = -1;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 1;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	else if((y->bfactor == -1)&&(x->bfactor == 1)){
		y->bfactor = 0;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 2;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	else if((y->bfactor == -1)&&(x->bfactor == -1)){
		y->bfactor = 1;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 1;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	else if((y->bfactor == -1)&&(x->bfactor == 0)){
		y->bfactor = 0;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 1;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	else if((y->bfactor == -2)&&(x->bfactor == -2)){
		y->bfactor = 1;//on met à jour le facteur d'équilibre du noeud courant
		x->bfactor = 0;//on met à jour le facteur d'équilibre du fils droit du noeud courant
	}
	return x;//on retourne le fils droit du noeud courant
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
	assert(x);// on vérifie que le noeud n'est pas null
	assert(get_left(x));// on vérifie que le fils gauche du noeud n'est pas null
	// quasi similaire à la fonction rotate_left
	struct tree_node_t *y = x->left;//on récupère le fils gauche du noeud courant
	struct tree_node_t *T2 = y->right;//on récupère le fils droit du fils gauche du noeud courant
	
	y->right = x;//on met le fils droit du fils gauche du noeud courant comme fils gauche du noeud courant
	x->left = T2;//on met le noeud courant comme fils droit du fils gauche du noeud courant

	if(x->bfactor == 2 && y->bfactor == 1)
	{
		x->bfactor = 0;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = 0;//on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	else if(x->bfactor == 2 && y->bfactor == 0)
	{
		x->bfactor = 1;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = -1;//	on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	else if(x->bfactor == 1 && y->bfactor == 1)
	{
		x->bfactor = -1;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = -1;//on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	else if(x->bfactor == 1 && y->bfactor == -1)
	{
		x->bfactor = 0;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = -2;//on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	else if(x->bfactor == 1 && y->bfactor == 0)
	{
		x->bfactor = 0;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = -1;//on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	else if(x->bfactor == 2 && y->bfactor == 2)
	{
		x->bfactor = -1;//on met à jour le facteurs d'équilibre du noeud courant
		y->bfactor = 0;//on met à jour le facteurs d'équilibre du fils gauche du noeud courant
	}
	return y;
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
	// - insertion du nœud
	if (tree_node_is_empty(curr)){//si le noeud courant est vide
		return new_tree_node(key, data);//on retourne un nouveau noeud
	}else if (preceed(key, curr->key)&&(get_left(curr)==NULL)){//si la clé est plus petite que la clé du noeud courant et que le fils gauche du noeud courant est vide
		set_left(curr, insert_into_tree_node(get_left(curr), key, data, balanced, preceed));//on insère le noeud à gauche du noeud courant
		decrease_bfactor(curr);//on décrémente le facteur d'équilibre du noeud courant
	}else if (preceed(curr->key, key)&&(get_right(curr)==NULL)){//si la clé est plus grande que la clé du noeud courant et que le fils droit du noeud courant est vide
		set_right(curr, insert_into_tree_node(get_right(curr), key, data, balanced, preceed));//on insère le noeud à droite du noeud courant
		increase_bfactor(curr);//on incrémente le facteur d'équilibre du noeud courant
	}else if(preceed(key, curr->key)&&(get_left(curr)!=NULL)){//si la clé est plus petite que la clé du noeud courant et que le fils gauche du noeud courant n'est pas vide
		int bfactor = get_bfactor(get_left(curr));//on récupère le facteur d'équilibre du fils gauche du noeud courant
		set_left(curr, insert_into_tree_node(get_left(curr), key, data, balanced, preceed));//on insère le noeud à gauche du noeud courant
		if (bfactor < get_bfactor(get_left(curr))){//si le facteur d'équilibre du fils gauche du noeud courant est plus petit que le facteur d'équilibre du fils gauche du fils gauche du noeud courant
			increase_bfactor(curr);//on décrémente le facteur d'équilibre du noeud courant
		}
	}else if(preceed(curr->key, key)&&(get_right(curr)!=NULL)){//si la clé est plus grande que la clé du noeud courant et que le fils droit du noeud courant n'est pas vide
		int bfactor = get_bfactor(get_right(curr));//on récupère le facteur d'équilibre du fils droit du noeud courant
		set_right(curr, insert_into_tree_node(get_right(curr), key, data, balanced, preceed));//on insère le noeud à droite du noeud courant
		if (bfactor > get_bfactor(get_right(curr))){//si le facteur d'équilibre du fils droit du noeud courant est plus grand que le facteur d'équilibre du fils droit du fils droit du noeud courant
			decrease_bfactor(curr);//on décrémente le facteur d'équilibre du noeud courant
		}
	}
	
	if (balanced) {
		// PARTIE 2 :
		// Gérer ici les rotations
		
		// - cas 1
		if (get_bfactor(curr) == 2 && get_bfactor(get_left(curr)) == 1){//si le facteur d'équilibre du noeud courant est égal à 2 et que le facteur d'équilibre du fils gauche du noeud courant est égal à 1
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 2
		}else if (get_bfactor(curr) == 2 && get_bfactor(get_left(curr)) == -1){//si le facteur d'équilibre du noeud courant est égal à 2 et que le facteur d'équilibre du fils gauche du noeud courant est égal à -1
			set_left(curr, rotate_left(get_left(curr)));//on effectue une rotation gauche sur le fils gauche du noeud courant
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 3
		}else if (get_bfactor(curr) == 2 && get_bfactor(get_left(curr)) == 0){//si le facteur d'équilibre du noeud courant est égal à 2 et que le facteur d'équilibre du fils gauche du noeud courant est égal à 0
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 4
		}else if (get_bfactor(curr) == 1 && get_bfactor(get_left(curr)) == 1){//si le facteur d'équilibre du noeud courant est égal à 1 et que le facteur d'équilibre du fils gauche du noeud courant est égal à 1
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 5
		}else if (get_bfactor(curr) == 1 && get_bfactor(get_left(curr)) == -1){
			set_left(curr, rotate_left(get_left(curr)));//on effectue une rotation gauche sur le fils gauche du noeud courant
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 6
		}else if (get_bfactor(curr) == 1 && get_bfactor(get_left(curr)) == 0){//si le facteur d'équilibre du noeud courant est égal à 1 et que le facteur d'équilibre du fils gauche du noeud courant est égal à 0
			curr = rotate_right(curr);//on effectue une rotation droite sur le noeud courant
		// - cas 7
		}else if (get_bfactor(curr) == -2 && get_bfactor(get_right(curr)) == -1){//si le facteur d'équilibre du noeud courant est égal à -2 et que le facteur d'équilibre du fils droit du noeud courant est égal à -1
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		// - cas 8
		}else if (get_bfactor(curr) == -2 && get_bfactor(get_right(curr)) == 1){//si le facteur d'équilibre du noeud courant est égal à -2 et que le facteur d'équilibre du fils droit du noeud courant est égal à 1
			set_right(curr, rotate_right(get_right(curr)));//on effectue une rotation droite sur le fils droit du noeud courant
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		// - cas 9
		}else if (get_bfactor(curr) == -2 && get_bfactor(get_right(curr)) == 0){//si le facteur d'équilibre du noeud courant est égal à -2 et que le facteur d'équilibre du fils droit du noeud courant est égal à 0
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		// - cas 10
		}else if (get_bfactor(curr) == -1 && get_bfactor(get_right(curr)) == -1){//si le facteur d'équilibre du noeud courant est égal à -1 et que le facteur d'équilibre du fils droit du noeud courant est égal à -1
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		// - cas 11
		}else if (get_bfactor(curr) == -1 && get_bfactor(get_right(curr)) == 1){//si le facteur d'équilibre du noeud courant est égal à -1 et que le facteur d'équilibre du fils droit du noeud courant est égal à 1
			set_right(curr, rotate_right(get_right(curr)));//on effectue une rotation droite sur le fils droit du noeud courant
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		// - cas 12
		}else if (get_bfactor(curr) == -1 && get_bfactor(get_right(curr)) == 0){//si le facteur d'équilibre du noeud courant est égal à -1 et que le facteur d'équilibre du fils droit du noeud courant est égal à 0
			curr = rotate_left(curr);//on effectue une rotation gauche sur le noeud courant
		}
	}
	return curr;//on retourne le noeud courant
}

/**
 * NB : Utiliser la fonction récursive insert_into_tree_node.
 */
void tree_insert(struct tree_t * T, void * key, void * data) {
	assert(T != NULL);//on vérifie que l'arbre n'est pas vide
	assert(key != NULL);//on vérifie que la clé n'est pas vide
	assert(data != NULL);//on vérifie que les données n'est pas vide
  set_root(T,insert_into_tree_node(T->root, key, data, T->balanced, T->preceed));//on insère le noeud dans l'arbre
  increase_tree_size(T);//on incrémente la taille de l'arbre
}

struct tree_node_t * tree_min(struct tree_node_t * curr) {
	assert(!tree_node_is_empty(curr));//on vérifie que le noeud courant n'est pas vide
	if(curr->left != NULL){//si le fils gauche du noeud courant n'est pas vide
		return tree_min(curr->left);//on appelle la fonction récursive tree_min sur le fils gauche du noeud courant
	}else{
		return curr;//on retourne le noeud courant
	}
}

struct tree_node_t * tree_max(struct tree_node_t * curr) {
	// similaire à tree_min
	assert(!tree_node_is_empty(curr));//on vérifie que le noeud courant n'est pas vide
	if(curr->right != NULL){//si le fils droit du noeud courant n'est pas vide
		return tree_max(curr->right);//on appelle la fonction récursive tree_max sur le fils droit du noeud courant
	}else{
		return curr;//on retourne le noeud courant
	}
}

struct tree_node_t * tree_find_node(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr));//on vérifie que le noeud courant n'est pas vide
	if (preceed(key, curr->key)){//si la clé est inférieure à la clé du noeud courant
		return tree_find_node(get_left(curr), key, preceed);//on appelle la fonction récursive tree_find_node sur le fils gauche du noeud courant
	}else if (preceed(curr->key, key)){//si la clé est supérieure à la clé du noeud courant
		return tree_find_node(get_right(curr), key, preceed);//on appelle la fonction récursive tree_find_node sur le fils droit du noeud courant
	}else{
		return curr;//on retourne le noeud courant
	}
}

struct tree_node_t * tree_find_predecessor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	assert(!tree_node_is_empty(curr));//on vérifie que le noeud courant n'est pas vide
	if (preceed(key, curr->key)){//si la clé est inférieure à la clé du noeud courant
		return tree_find_predecessor(get_left(curr), key, preceed);//on appelle la fonction récursive tree_find_predecessor sur le fils gauche du noeud courant
	}else if (preceed(curr->key, key)){//si la clé est supérieure à la clé du noeud courant
		struct tree_node_t * tmp = tree_find_predecessor(get_right(curr), key, preceed);//on appelle la fonction récursive tree_find_predecessor sur le fils droit du noeud courant
		if (tree_node_is_empty(tmp)){//si le noeud retourné par la fonction récursive est vide
			return curr;//on retourne le noeud courant
		}else{
			return tmp;//on retourne le noeud retourné par la fonction récursive
		}
	}else{
		return tree_max(get_left(curr));//on retourne le noeud maximum du fils gauche du noeud courant
	}
}

struct tree_node_t * tree_find_successor(struct tree_node_t * curr, void * key, int (*preceed)(const void *, const void *)) {
	// similaire à tree_find_predecessor
	assert(!tree_node_is_empty(curr));//on vérifie que le noeud courant n'est pas vide
	if (preceed(key, curr->key)){//si la clé est inférieure à la clé du noeud courant
		struct tree_node_t * tmp = tree_find_successor(get_left(curr), key, preceed);//on appelle la fonction récursive tree_find_successor sur le fils gauche du noeud courant
		if (tree_node_is_empty(tmp)){//si le noeud retourné par la fonction récursive est vide
			return curr;//on retourne le noeud courant
		}else{
			return tmp;//on retourne le noeud retourné par la fonction récursive
		}
	}else if (preceed(curr->key, key)){//si la clé est supérieure à la clé du noeud courant
		return tree_find_successor(get_right(curr), key, preceed);//on appelle la fonction récursive tree_find_successor sur le fils droit du noeud courant
	}else{//si la clé est égale à la clé du noeud courant
		return tree_min(get_right(curr));//on retourne le noeud minimum du fils droit du noeud courant
	}
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
	// - suppression du nœud
	*data = NULL;//on met NULL dans la variable data
	if(tree_node_is_empty(curr)){//si le noeud courant est vide
		data = get_tree_node_data(curr);//on met la donnée du noeud courant dans la variable data
		free(curr);//on libère le noeud courant
		return NULL;//on retourne NULL
	}else if(preceed(key, curr->key)){//si la clé est inférieure à la clé du noeud courant
		set_left(curr, remove_tree_node(get_left(curr), key, data, balanced, preceed));
	}else if(preceed(curr->key, key)){//si la clé est supérieure à la clé du noeud courant
		set_right(curr, remove_tree_node(get_right(curr), key, data, balanced, preceed));
	}else{//si la clé est égale à la clé du noeud courant
		*data = curr->data;//on met la donnée du noeud courant dans la variable data
		if(get_left(curr) == NULL){//si le fils gauche du noeud courant est vide
			struct tree_node_t * tmp = get_right(curr);//on met le fils droit du noeud courant dans une variable temporaire
			free(curr);//on libère le noeud courant
			return tmp;//on retourne la variable temporaire
		}else if(get_right(curr) == NULL){//si le fils droit du noeud courant est vide
			struct tree_node_t * tmp = get_left(curr);//on met le fils gauche du noeud courant dans une variable temporaire
			free(curr);//on libère le noeud courant
			return tmp;//on retourne la variable temporaire
		}else{//si les deux fils du noeud courant ne sont pas vides
			struct tree_node_t * tmp = tree_min(get_right(curr));//on met le noeud minimum du fils droit du noeud courant dans une variable temporaire
			curr->key = tmp->key;//on met la clé du noeud temporaire dans la clé du noeud courant
			curr->data = tmp->data;//on met la donnée du noeud temporaire dans la donnée du noeud courant
			set_right(curr, remove_tree_node(get_right(curr), tmp->key, data, balanced, preceed));//on appelle la fonction récursive remove_tree_node sur le fils droit du noeud courant
		}
	}

	if (balanced && curr) {
		// PARTIE 2 :
		// Gérer ici les rotations
		if ((curr->bfactor == 2)) {//si le facteur d'équilibre du noeud courant est égal à 2
			if (curr->left->bfactor == 1){//si le facteur d'équilibre du fils gauche du noeud courant est égal à 1
				curr = rotate_right(curr);//on fait une rotation droite
			}else if (curr->left->bfactor == -1) {//si le facteur d'équilibre du fils gauche du noeud courant est égal à -1
				//rotation gauche droite
				curr->left = rotate_left(curr->left);//on fait une rotation gauche sur le fils gauche du noeud courant
				curr = rotate_right(curr);//on fait une rotation droite sur le noeud courant
			}else{//si le facteur d'équilibre du fils gauche du noeud courant est égal à 0
				curr = rotate_right(curr);//on fait une rotation droite
			}
		}else if (curr->bfactor == -2) {//si le facteur d'équilibre du noeud courant est égal à -2
			if (curr->right->bfactor == -1) {//si le facteur d'équilibre du fils droit du noeud courant est égal à -1
				curr = rotate_left(curr);//on fait une rotation gauche
			}else if (curr->right->bfactor == 1){//si le facteur d'équilibre du fils droit du noeud courant est égal à 1
				curr->right = rotate_right(curr->right);//on fait une rotation droite sur le fils droit du noeud courant
				curr = rotate_left(curr);//on fait une rotation gauche sur le noeud courant
			}else{//si le facteur d'équilibre du fils droit du noeud courant est égal à 0
				curr = rotate_left(curr);//on fait une rotation gauche
			}
		}
	}
	return curr;//on retourne le noeud courant
}

/**
 * NB : Utiliser la fonction récursive remove_tree_node.
 */
void * tree_remove(struct tree_t * T, void * key) {
    void * data = NULL;//on met NULL dans la variable data
    T->root = remove_tree_node(T->root, key, &data, T->balanced, T->preceed);//on appelle la fonction récursive remove_tree_node sur la racine de l'arbre
	decrease_tree_size(T);//on décrémente la taille de l'arbre
	return data;//on retourne la variable data
}
