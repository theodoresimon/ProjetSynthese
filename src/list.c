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
static struct list_node_t *new_list_node(void *data)
{
	struct list_node_t *newListNode = malloc(sizeof(struct list_node_t));
	newListNode->data = data;
	newListNode->successor = NULL;
	newListNode->predecessor = NULL;
	return newListNode;
}

void *get_list_node_data(const struct list_node_t *node)
{
	return node->data;
}

struct list_node_t *get_successor(const struct list_node_t *node)
{
	return node->successor;
}

struct list_node_t *get_predecessor(const struct list_node_t *node)
{
	return node->predecessor;
}

void set_list_node_data(struct list_node_t *node, void *newData)
{
	node->data = newData;
}

void set_successor(struct list_node_t *node, struct list_node_t *newSucc)
{
	node->successor = newSucc;
}

void set_predecessor(struct list_node_t *node, struct list_node_t *newPred)
{
	node->predecessor = newPred;
}

/********************************************************************
 * list_t
 ********************************************************************/

struct list_t *new_list(void (*viewData)(const void *), void (*freeData)(void *))
{
	struct list_t *newList = malloc(sizeof(struct list_t));
	newList->head = NULL;
	newList->tail = NULL;
	newList->numelm = 0;
	newList->viewData = viewData;
	newList->freeData = freeData;
	return newList;
}

int list_is_empty(struct list_t *L)
{
	return (L->numelm == 0);
}

int get_list_size(const struct list_t *L)
{
	return L->numelm;
}

struct list_node_t *get_list_head(const struct list_t *L)
{
	return L->head;
}

struct list_node_t *get_list_tail(const struct list_t *L)
{
	return L->tail;
}

void increase_list_size(struct list_t *L)
{
	L->numelm++;
}

void decrease_list_size(struct list_t *L)
{
	if (list_is_empty(L))
	{
		ShowMessage("src.list.c:decrease_list_size : La liste est vide, inutile de la decrementer !",1);
	}
	L->numelm--;
}

void set_list_size(struct list_t *L, int newSize)
{
	L->numelm = newSize;
}

void set_head(struct list_t *L, struct list_node_t *newHead)
{
	if (list_is_empty(L))//illogique de donner une liste vide mais on sait jamais 
	{
		// Si la liste est vide, on ne peut pas modifier la tête de la liste
		// car la tête est NULL, et on ne peut pas modifier un pointeur NULL
		// On affiche donc un message d'erreur et on quitte la fonction
		ShowMessage("src.list.c:set_head : La liste est vide !", 1);
	}
	else
	{
		L->head = newHead;
	}
}

void set_tail(struct list_t *L, struct list_node_t *newTail)
{
	if (list_is_empty(L))
	{
		// Meme raisonnement que pour set_head
		ShowMessage("src.list.c:set_tail : La liste est vide !",1);
	}
		L->tail = newTail;
}

void delete_list(struct list_t *L, int deleteData)
{
	struct list_node_t* current = L->head;
	struct list_node_t* sucessor_node;//permettra de prendre les noeuds suivants de current et de les supprimer les uns après les autres 
	if (list_is_empty(L))
	{
		ShowMessage("src.list.c:delete_list : La liste est déjà vide !", 1);
	}
	else
		{
			while(current != NULL){//permet de parcourir les noeuds tant que la liste n'est pas vide
				sucessor_node = get_successor(current) //on se sert de ce noeud pour ne pas perdre le successeur de la tête de liste, on libère donc la liste de la tête vers la queue de la liste 
				if ((deleteData == 1) && (L->freeData != NULL)){//comme descrit dans la définition de la fonction
					L->freeData(current->data);//appelle de la fonction freedata pour libérer la data du noeud courrent
				}
				free(current);// libère le noeud courant
				current = sucessor_node; //current étant libérer on le remplace par son successeur stocké au préalable
			}
			 

	}
	free(L);//quand tous les noeuds de la liste sont libérer, on libère la liste
}

void view_list(const struct list_t *L)
{
	for (struct list_node_t *temp = get_list_head(L); temp != NULL; temp = get_successor(temp))//parcours toute la liste pour afficher la data de chaque noeuds jusqu'a ce que temp prenne la valeur NULL(quand il n'y a plus de successeur) 
	{
		L->viewData(temp->data);// appelle de la fonction viewData pour afficher la data du noeud temporaire
	}
}

void list_insert_first(struct list_t *L, void *data)
{
	struct list_node_t *newListNode = new_list_node(data);
	if (list_is_empty(L))
	{	// si la liste est vide alors la tête et la queue de la liste sont les même 
		set_head(L, newListNode);
		set_tail(L, newListNode);
	}
	else 
	{
		newListNode->successor = L->head;//sinon la tête de liste actuel  deviens le successor du nouveau noeuds
		L->head->predecessor = newListNode;//puis on intègre au predecesseur de la tête de liste actuel le noeud que l'on veux intégré à la liste(newListNode)
		set_head(L, newListNode);// et enfin on intère le noeud à la tête de la Liste
	}
	increase_list_size(L);//on oublie pas d'incrémenté la liste étant donné que l'on rajoute un noeud
}

void list_insert_last(struct list_t *L, void *data)
{
	struct list_node_t *newListNode = new_list_node(data);
	if (list_is_empty(L))
	{	// si la liste est vide alors la tête et la queue de la liste sont les même
		set_head(L, newListNode);
		set_tail(L, newListNode);
	}
	else
	{
		L->tail->successor = newListNode;//sinon la queue de liste actuel reçoit comme successor le nouveau noeuds
		newListNode->predecessor = L->tail;//puis on intègre au predecesseur du nouveau noeud, la queue de la liste
		L->tail = newListNode;// et enfin on intère le noeud à la queue de la Liste et donc deviens le dernier élément de la liste 
	}
	increase_list_size(L);//on oublie pas d'incrémenté la liste étant donné que l'on rajoute un noeud
}

void list_insert_after(struct list_t *L, void *data, struct list_node_t *ptrelm)
{
	struct list_node_t *newListNode = new_list_node(data);//on crée le noeud avec la data que l'on veux intégré après le noeud ptrelm
	newListNode->predecessor = ptrelm;//le prédecesseur du nouveau noeud est le noeud ptrelm
	ptrelm->successor->predecessor = newListNode;//avant de modifier le successeur de ptrelm, il faut que le noeud suivant ptrelm reçoivent notre nouveau noeuds en prédeccesseur  
	newListNode->successor = ptrelm->successor;// le successeur du nouveau noeud sera donc le successeur du noeud ptrelm étant donné que l'on veux le mettre après ptrelm 
	ptrelm->successor = newListNode;//on modifie le sucesseur de ptrelm pour que ce soit le nouveaunoeud que l'on souhaite ajouté   
	increase_list_size(L);// on rajoute un element donc on incrémente la liste 
}

void *list_remove_first(struct list_t *L)
{
	assert(get_list_head(L));//on vérifie que la liste n'est pas vide 
	struct list_node_t *temp = get_list_head(L); //on crée un noeud temporaire pour recevoir la tête de liste
	if(get_successor(L->head->successor) == NULL)//si le successeur de la tête est null alors la liste n'a qu'un élément et on ne pourra modifier la tête de L avec son successeur étant donné qu'il n'y en a pas
	{
		L->head = NULL;//si on supprime le seul élément de la liste alors la queue et la tête pointe sur null
		L->tail = NULL;
	}else{ //sinon la liste à plus d'1 élément  
		set_head(L, get_successor(temp));//la tête de la liste recoit le successeur de sa tête actuel comme nouvelle tête 
		set_predecessor(get_list_head(L), NULL);//le prédeccesseur de la nouvelle tête ne doit donc plus pointé sur le noeud que l'on veux supprimé et donc pointé sur null
	}
	decrease_list_size(L);//étant donnée que l'on retire un élément il faut décrémenté numelm
	void *data = get_list_node_data(temp);//on stocke la data dans data pour ne pas perdre la donnée
	free(temp);//on libère l'ancienne tête de L en ayant au préalable supprimé les liens où il y avait une relation avec la Liste ou son successeur
	return data;
}

void *list_remove_last(struct list_t *L)// quasi similaire à remove_first
{
	assert(get_list_head(L));//on vérifie que la liste n'est pas vide 
	struct list_node_t *temp = get_list_tail(L); //on crée un noeud temporaire pour recevoir la queue de la liste
	if(get_successor(L->head->successor) == NULL)//si le successeur de la tête est null alors la liste n'a qu'un élément et on ne pourra pas modifier la queue de L avec son prédeccesseur étant donné qu'il n'y en a pas
	{
		L->head = NULL;//si on supprime le seul élément de la liste alors la queue et la tête pointe sur null
		L->tail = NULL;
	}else{ //sinon la liste à plus d'1 élément
		set_tail(L, get_predecessor(temp));//la queue de la liste recoit le predeccesseur de sa queue actuel comme nouvelle queue 
		set_successor(temp, NULL);//le successeur de la nouvelle queue ne doit donc plus pointé sur le noeud que l'on veux supprimé et donc pointé sur null
	}
	decrease_list_size(L);//étant donnée que l'on retire un élément il faut décrémenté numelm
	void *data = get_list_node_data(temp);//on stocke la data dans data pour ne pas perdre la donnée
	free(temp);//on libère l'ancienne tête de L en ayant au préalable supprimé les liens où il y avait une relation avec la Liste ou son successeur
	return data;
}

void *list_remove_node(struct list_t *L, struct list_node_t *node)
{
	assert(get_list_head(L) && get_list_tail(L) && (get_list_head(L) != get_list_tail(L)) );// on vérifie que la liste à bien au moins 2 éléments sinon inutile d'appeler remove_node
	void *data = get_list_node_data(node);//on stocke la data dans data pour ne pas perdre la donnée  
	if(node->predecessor == NULL)//si le predeccesseur du noeud node est null alors node est la tête de la liste donc on doit faire les modifications adéquates
	{
		L->head = node->successor;// le successeur de node deviens donc la nouvelle tête 
		node->successor->predecessor = NULL;//le pointeur predeccessor du  successeur du noeud node doit donc pointé sur NULL étant donné que c'est la nouvelle tête de liste 
	}else if(node->successor == NULL){//si le successeur du noeud node est null alors node est la queue de la liste donc on doit faire les modifications adéquates
		L->tail = node->predecessor;//la nouvelle queue de liste devient donc le predeccesseur de node 
		node->predecessor->successor = NULL;//le pointeur successor du predeccesseur de node doit donc pointé que NULL
	}else //si ce n'est ni la tête de liste ni la queue de liste alors quelque soit sa place les modifications resteront les même    
	{
		node->predecessor->successor = node->successor;//le pointeur successeur du noeud précédant le noeud node pointe désormais sur le sucesseur du noeud node 
		node->successor->predecessor = node->predecessor;//le pointeur predecceur du noeud suivant node pointe désormais sur le predeccesseur du noeud node
	}
	decrease_list_size(L);//on supprime le noeud node de la liste donc il faut decrementer la liste
	free(node);//on libère le noeuds que l'on souhaite supprimé 
	return data;
}