/*
 ============================================================================
 Name        : hw_c2.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include "LinkedList.h"

typedef struct Node_t
{
	element elem;
	struct Node_t *next;
}Node;

struct List_t
{
	Node* head;
	copyFunction copyElement;
	freeFunction freeElement;
	equalFunction compareElements;
	printFunction printElement;
	getCategoryFunction getCategory;
	int list_len;
};

List createLinkedList(copyFunction copy, freeFunction free, equalFunction compare, printFunction print,
		getCategoryFunction getCategory){
	if (copy == NULL || free == NULL || compare == NULL || print == NULL || getCategory  == NULL){
		return NULL;
	}
	List list = (List) malloc(sizeof(struct List_t));
	if (list == NULL){
		return NULL;
	}
	list->head = NULL;
	list->list_len = 0;
	list->copyElement = copy;
	list->freeElement = free;
	list->compareElements = compare;
	list->printElement = print;
	list->getCategory = getCategory;
	return list;
}

status destroyList(List p_list){
	if (p_list == NULL){
		return failure;
	}
	while (p_list->list_len != 0){
		deleteNode(p_list, p_list->head->elem);
	}
	free(p_list);
	return success;
}

Node *CreateNode(element newElem){
	if (newElem == NULL){
		return NULL;
	}
	Node *node = (Node*) malloc(sizeof(Node));
	if (node==NULL){
		return NULL;
	}
	node->elem = newElem;
	node->next = NULL;
	return node;
}

status appendNode(List p_list, element Element){
	if (p_list == NULL || Element == NULL){
		return failure;
	}
	element newElement = p_list->copyElement(Element);
	if (newElement == NULL){ //memory problem
		return failure;
	}
	if (p_list->list_len == 0){
		p_list->head = CreateNode(newElement);
		if (p_list->head == NULL){ //memory problem
			p_list->freeElement(newElement);
			return failure;
		}
		p_list->list_len = 1;
		return success;
	}
	else{
		Node *temp;
		temp = p_list->head;
		while (temp->next != NULL){
			temp = temp->next;
		}
		temp->next = CreateNode(newElement);
		if (temp->next == NULL){ //memory problem
			p_list->freeElement(newElement);
			return failure;
		}
		p_list->list_len ++;
		return success;
	}
}

status deleteNode(List p_list, element Element){
	if (p_list == NULL || Element == NULL || p_list->list_len == 0){
		return failure;
	}
	Node *temp;
	temp = p_list->head->next;
	char *category = p_list->getCategory(Element);
	if (p_list->compareElements(category, p_list->head->elem) == 0){
		p_list->freeElement(p_list->head->elem);
		free(p_list->head);
		p_list->head = temp;
		p_list->list_len --;
		free(category);
		return success;
	}
	temp = p_list->head;
	while(temp->next != NULL){
		if (p_list->compareElements(category, temp->next->elem) == 0){
			Node *tempDelNext;
			tempDelNext = temp->next->next;
			p_list->freeElement(temp->next->elem);
			free(temp->next);
			temp->next = tempDelNext;
			p_list->list_len --;
			free(category);
			return success;
		}
		temp = temp->next;
	}
	return failure;
}

status displayList(List p_list){
	if (p_list == NULL || p_list->list_len == 0){
		return failure;
	}
	Node *temp;
	temp = p_list->head;
	while(temp != NULL){
		if (p_list->printElement(temp->elem) == failure){
			return failure;
		}
		temp = temp->next;
	}
	return success;
}

element searchByKeyInList(List p_list, element key){
	if (p_list == NULL || key == NULL){
		return NULL;
	}
	Node *temp;
	temp = p_list->head;
	while(temp != NULL){
		if (p_list->compareElements(key, temp->elem) == 0){
			element newElem = p_list->copyElement(temp->elem);
			return newElem;
		}
		temp = temp->next;
	}
	return NULL;
}
