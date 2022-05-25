#ifndef DEFS_H
#define DEFS_H

//possible return values
typedef enum e_bool { false,true } bool;
typedef enum e_status { success, failure, memory } status;
//generic ADT
typedef void * element;
typedef element(*copyFunction) (element);
typedef status(*freeFunction) (element);
typedef status(*printFunction) (element);
typedef int(*equalFunction) (element, element); // 1 first bigger, 0 same, -1 second bigger, -2 memory problem or uninitialized value
typedef char*(*getCategoryFunction)(element);
typedef int(*getAttackFunction)(element firstElem , element secondElem, int* attackFirst, int *attackSecond); // return the first attack - second attack. also produce the attack of each element, The function transmits the information through the pointers received in the function.

#endif
