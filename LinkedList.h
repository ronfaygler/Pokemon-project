/*LinkedList.h - linked list ADT module header file
 this module defines a pointer to linked list type and implements some linked list manipulation functions.*/
#ifndef LinkedList_H
#define LinkedList_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

typedef struct List_t *List;

//creates linked list and returns pointer to this list, if fails returns NULL
List createLinkedList(copyFunction copy, freeFunction free, equalFunction compare, printFunction print,
		getCategoryFunction getCategory);
//deletes list and returns if it has succeed
status destroyList(List p_list);
//inserts element to list and returns if it has succeed
status appendNode(List p_list, element Element);
//removes the element from the list and returns if it has succeed
status deleteNode(List p_list, element Element);
//prints the details of the list and returns a status if it has succeed
status displayList(List p_list);
//checks if the key is in the list, if it is, returns the element that contains this key. if its not, returns NULL
element searchByKeyInList(List p_list, element key);

#endif
