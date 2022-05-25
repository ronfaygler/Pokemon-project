#include "BattleByCategory.h"
#include "LinkedList.h"
#include "MaxHeap.h"
#include "Defs.h"

struct battle_s{
	List categoryList;
	char **nameList;
	int numCategories;
	equalFunction equalElement;
	copyFunction copyElement;
	freeFunction freeElement;
	getCategoryFunction getCategory;
	getAttackFunction getAttack;
	printFunction printElement;
};

status castDestroyHeap(element elem){
	Heap heap = (Heap)elem;
	return destroyHeap(heap);
}

status castPrintHeap(element elem){
	Heap heap = (Heap)elem;
	return printHeap(heap);
}

char *castGetHeapId(element elem){
	Heap heap = (Heap)elem;
	return getHeapId(heap);
}

element copyHeap(element elem){ //shallow copy
	if(elem == NULL){
		return NULL;
	}
	return elem;
}

int equalHeap(element elem1, element elem2){ // 1=not equal
	if (elem1 == NULL || elem2 == NULL){
		return -2;
	}
	char *category = (char *)elem1;
	Heap heap = (Heap)elem2;
	char *heapName = getHeapId(heap);
	if (heapName == NULL){ //memory problem
		return -2;
	}
	if(strcmp(heapName, category) == 0){
		free(heapName);
		return 0;
	}
	else{
		free(heapName);
		return 1;
	}
}

Battle createBattleByCategory(int capacity,int numberOfCategories,char* categories,equalFunction equalElement,
		copyFunction copyElement,freeFunction freeElement,getCategoryFunction getCategory,getAttackFunction getAttack,printFunction printElement){
	if (categories == NULL || equalElement == NULL || copyElement == NULL || freeElement == NULL ||
			getCategory == NULL || getAttack == NULL || printElement == NULL){
		return NULL;
	}
	Battle battle = (Battle) malloc(sizeof(struct battle_s));
	if (battle == NULL){
		return NULL;
	}
	battle->categoryList = createLinkedList(copyHeap, castDestroyHeap, equalHeap, castPrintHeap, castGetHeapId);
	if (battle->categoryList == NULL){
		free(battle);
		return NULL;
	}
	battle->nameList = (char **) malloc(numberOfCategories * sizeof(char*));
	if (battle->nameList == NULL){
		destroyList(battle->categoryList);
		free(battle);
		return NULL;
	}
	char lim[] = ",\r\n";
	int i = 0;
	char *category = strtok(categories, lim);
	while (category != NULL){
		Heap heap  = CreateHeap(copyElement, freeElement, printElement, equalElement, capacity, category);
		if (heap == NULL){
			free(battle->nameList);
			destroyList(battle->categoryList);
			free(battle);
			return NULL;
		}
		battle->nameList[i] = category;
		if (appendNode(battle->categoryList, heap) == failure){
			destroyHeap(heap);
			free(battle->nameList);
			destroyList(battle->categoryList);
			free(battle);
			return NULL;
		}
		category = strtok(NULL, lim);
		i ++;
	}
	battle->equalElement = equalElement;
	battle->copyElement = copyElement;
	battle->freeElement = freeElement;
	battle->getCategory = getCategory;
	battle->getAttack = getAttack;
	battle->printElement = printElement;
	battle->numCategories = numberOfCategories;
	return battle;
}

status destroyBattleByCategory(Battle battle){
	if (battle == NULL){
		return failure;
	}
	free(battle->nameList);
	destroyList(battle->categoryList);
	free(battle);
	return success;
}

status insertObject(Battle battle, element elem){
	if(battle == NULL || elem == NULL){
		return failure;
	}
	char *category = battle->getCategory(elem);
	element heap = searchByKeyInList(battle->categoryList, (element)category);
	if (heap == NULL){ //heap with name of the category not found
		return failure;
	}
	free(category);
	return (insertToHeap((Heap)heap, elem)); //no need to enter a copy of the element, because the insertToHeap function will make the copy.
}

void displayObjectsByCategories(Battle battle){
	if (battle == NULL){
		return;
	}
	displayList(battle->categoryList);
}

element removeMaxByCategory(Battle battle, char *category){
	if(battle == NULL || category == NULL){
		return NULL;
	}
	element heap = searchByKeyInList(battle->categoryList, (element) category);
	if (heap == NULL){ //heap with name of the category not found
		return NULL;
	}
	return PopMaxHeap((Heap)heap); // need to do free after in main.
}

int getNumberOfObjectsInCategory(Battle battle, char *category){
	if(battle == NULL || category == NULL){
		return -1;
	}
	element heap = searchByKeyInList(battle->categoryList, (element)category);
	if (heap == NULL){ //heap with name of the category not found
		return -1;
	}
	return getHeapCurrentSize((Heap)heap);
}

element fight(Battle battle, element elem2){
	if (battle == NULL || elem2 == NULL){
		return NULL;
	}
	int attack1 = 0;
	int attack2 = 0;
	int maxAttack1;
	element top = NULL;
	element maxElem = NULL;
	for (int i=0; i < battle->numCategories; i++){
		element heap = searchByKeyInList(battle->categoryList, (element)(battle->nameList[i])); // the heap will never be null
		top = TopMaxHeap((Heap)heap);
		if (top == NULL){
			if(getHeapCurrentSize((Heap)heap) != 0){ // memory problem
				return NULL;
			}
		}
		if (top != NULL){
			if (i == 0){
				maxAttack1 = battle->getAttack(top, elem2, &attack1, &attack2);
				maxElem = battle->copyElement(top);
				if (maxElem == NULL){ //memory problem
					return NULL;
				}
			}
			else if (battle->getAttack(top, elem2, &attack1, &attack2) > maxAttack1){
				maxAttack1 = battle->getAttack(top, elem2, &attack1, &attack2);
				battle->freeElement(maxElem);
				maxElem = battle->copyElement(top);
				if (maxElem == NULL){ //memory problem
					return NULL;
				}
			}
			battle->freeElement(top);
		}
	}
	printf("You choose to fight with:\n");
	battle->printElement(elem2);
	if(maxElem == NULL){ //no elements in battle's heaps
		return NULL;
	}
	battle->getAttack(maxElem, elem2, &attack1, &attack2);
	printf("The final battle between:\n");
	battle->printElement(elem2);
	printf("In this battle his attack is :%d\n\nagainst ",attack2);
	battle->printElement(maxElem);
	printf("In this battle his attack is :%d\n\n", attack1);
	if (attack1 > attack2){
		printf("THE WINNER IS:\n");
		battle->printElement(maxElem);
		battle->freeElement(elem2);
		return maxElem;
	}
	else if(attack1 == attack2){
		printf("IT IS A DRAW.\n");
		battle->freeElement(elem2);
		return maxElem;
	}
	else{
		printf("THE WINNER IS:\n");
		battle->printElement(elem2);
		battle->freeElement(maxElem);
		return elem2;
	}
}
