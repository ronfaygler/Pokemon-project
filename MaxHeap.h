/*MaxHeap.h - Max heap ADT module header file
 this module defines a pointer to heap type and implements some heap manipulation functions.*/
#ifndef MaxHeap_H
#define MaxHeap_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Defs.h"

typedef struct Heap_t *Heap;

//creates heap and returns pointer to this heap, if fails returns NULL
Heap CreateHeap(copyFunction copy, freeFunction free,printFunction print, equalFunction compare, int capacity, char *name);
//deletes heap and returns if it has succeed
status destroyHeap(Heap heap);
//inserts element to heap and returns if it has succeed
status insertToHeap(Heap heap, element Element);
//prints the details of the heap and returns a status if it has succeed
status printHeap(Heap heap);
//removes the element with the biggest value in the heap, and returns this element, if fails returns NULL
element PopMaxHeap(Heap heap);
//returns the element with the biggest value in the heap, if fails returns NULL
element TopMaxHeap(Heap heap);
//returns the name of the heap, if fails returns NULL
char *getHeapId(Heap heap);
//returns the size of the heap currently, if fails returns -1
int getHeapCurrentSize(Heap heap);

#endif
