/*BattleByCategory.h - Battle ADT module header file
 this module defines a pointer to battle type and implements some battle manipulation functions.*/
#ifndef BATTLEBYCATEGORY_H_
#define BATTLEBYCATEGORY_H_
#include "Defs.h"

typedef struct battle_s* Battle;

//create battle and returns pointer to this battle, if fails returns NULL
Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement);
//delete battle and returns if it has succeed
status destroyBattleByCategory(Battle b);
//inserts element to the battle and returns if it has succeed
status insertObject(Battle b, element elem);
//prints the details of the battle
void displayObjectsByCategories(Battle b);
//removes the element with the biggest value in the category, and returns this element. if fails returns NULL
element removeMaxByCategory(Battle b,char* category);
//returns the size of the category in the battle currently, if fails returns -1
int getNumberOfObjectsInCategory(Battle b,char* category);
/*makes fight with the received element, against the most powerful element for this fight,
 * that is in the battle, and returns the winner. if fails returns NULL*/
element fight(Battle b,element elem);

#endif /* BATTLEBYCATEGORY_H_ */
