#include "MaxHeap.h"

struct Heap_t
{
	char *heapName;
	element *array;
	int maxCapacity;
	int currLen;
	copyFunction copyElement;
	freeFunction freeElement;
	equalFunction compareElements;
	printFunction printElement;
};

Heap CreateHeap(copyFunction copy, freeFunction free,printFunction print, equalFunction compare, int capacity, char *name){
	if (copy == NULL || free == NULL || print == NULL || compare == NULL || name == NULL){
		return NULL;
	}
	Heap heap = (Heap) malloc(sizeof(struct Heap_t));
	if (heap == NULL){
		return NULL;
	}
	heap->array =(element *) malloc(0);
	if (heap->array == NULL){
		free(heap);
		return NULL;
	}
	heap->heapName = (char *) malloc(strlen(name)+1);
	if (heap->heapName == NULL){
		free(heap->array);
		free(heap);
		return NULL;
	}
	strcpy(heap->heapName, name);
	heap->maxCapacity = capacity;
	heap->currLen = 0;
	heap->copyElement = copy;
	heap->freeElement = free;
	heap->printElement = print;
	heap->compareElements = compare;
	return heap;
}

status destroyHeap(Heap heap){
	if (heap == NULL){
		return failure;
	}
	if (heap->currLen == 0){
		free(heap->heapName);
		free(heap->array);
		free(heap);
		return success;
	}
	int heapLen = heap->currLen;
	for (int i=0; i < heapLen; i++){
		heap->freeElement(heap->array[i]);
		heap->currLen --;
	}
	free(heap->heapName);
	free(heap->array);
	free(heap);
	return success;
}

void HeapIncreaseKey(Heap heap, int i, element newElem){
	if (heap == NULL || newElem == NULL){
		return;
	}
	while(i > 0 && (heap->compareElements(heap->array[(i-1)/2], heap->array[i]) == -1)){
		element temp;
		temp = heap->array[(i-1)/2];
		heap->array[(i-1)/2] = heap->array[i];
		heap->array[i] = temp;
		i = (i-1)/2;
	}
}

status maxHeapInsert(Heap heap, element newElem){
	if (heap == NULL || newElem  == NULL){
		return failure;
	}
	heap->array = (element *) realloc(heap->array, (heap->currLen+1) * sizeof(element));
	if (heap->array == NULL){
		return failure;
	}
	heap->currLen++;
	heap->array[heap->currLen-1] = newElem;
	HeapIncreaseKey(heap, heap->currLen-1, newElem);
	return success;
}

status insertToHeap(Heap heap, element Element){
	if (heap == NULL || Element == NULL){
		return failure;
	}
	if (heap->currLen == heap->maxCapacity){
		return failure;
	}
	element newElement = heap->copyElement(Element);
	if (newElement == NULL){
		return failure;
	}
	if (heap->currLen == 0){
		heap->array = (element *) realloc(heap->array, heap->currLen+1 * sizeof(element));
		if (heap->array == NULL){
			heap->freeElement(newElement);
			return failure;
		}
		heap->array[0] = newElement;
		heap->currLen++;
		return success;
	}
	return maxHeapInsert(heap, newElement);
}

void MaxHeapify(Heap heap, int i){
	if (heap == NULL){
		return;
	}
	int l, r;
	l = 2*(i+1)-1;
	r = 2*(i+1);
	int biggest;
	if (l < heap->currLen && heap->compareElements(heap->array[l], heap->array[i]) == 1){
		biggest = l;
	}
	else{
		biggest = i;
	}
	if (r < heap->currLen && heap->compareElements(heap->array[r], heap->array[biggest]) == 1){
		biggest = r;
	}
	if (biggest != i){
		element temp;
		temp = heap->array[i];
		heap->array[i] = heap->array[biggest];
		heap->array[biggest] = temp;
		MaxHeapify(heap, biggest);
	}
}

element PopMaxHeap(Heap heap){
	if (heap == NULL){
		return NULL;
	}
	if (heap->currLen == 0){
		return NULL;
	}
	element maxElem;
	maxElem = heap->copyElement(heap->array[0]);
	if (maxElem == NULL){
		return NULL;
	}
	heap->freeElement(heap->array[0]);
	if (heap->currLen == 1){
		heap->currLen = 0;
		heap->array = (element *) realloc(heap->array, 0);
		return maxElem;
	}
	heap->array[0] = heap->copyElement(heap->array[heap->currLen-1]);
	if (heap->array[0] == NULL){
		heap->freeElement(maxElem);
		return NULL;
	}
	heap->freeElement(heap->array[heap->currLen-1]);
	heap->currLen --;
	heap->array = (element *) realloc(heap->array, (heap->currLen) * sizeof(element));
	if (heap->array == NULL){
		heap->freeElement(maxElem);
		heap->freeElement(heap->array[0]);
		return NULL;
	}
	MaxHeapify(heap, 0);
	return maxElem;
}

status printHeap(Heap heap){
	if (heap == NULL){
		return failure;
	}
	if (heap->currLen == 0){
		printf("%s:\nNo elements.\n\n", heap->heapName);
		return success;
	}
	Heap newHeap = CreateHeap(heap->copyElement, heap->freeElement, heap->printElement, heap->compareElements, heap->maxCapacity, heap->heapName);
	if (newHeap == NULL){
		return failure;
	}
	newHeap->array = (element *) realloc(newHeap->array, heap->currLen * sizeof(element));
	if (newHeap->array == NULL){
		destroyHeap(newHeap);
		return failure;
	}
	newHeap->currLen = heap->currLen;
	for (int i=0; i < newHeap->currLen; i++){
		newHeap->array[i] = heap->copyElement((Heap)heap->array[i]);
		if (newHeap->array[i] == NULL){
			free(newHeap->array);
			destroyHeap(newHeap);
			return failure;
		}
	}
	element top;
	printf("%s:\n", newHeap->heapName);
	for(int i=0; i < heap->currLen; i++){
		top = PopMaxHeap(newHeap);
		printf("%d. ", i+1);
		heap->printElement(top);
		heap->freeElement(top);
	}
	destroyHeap(newHeap);
	return success;
}

element TopMaxHeap(Heap heap){
	if (heap == NULL){
		return NULL;
	}
	if (heap->currLen == 0){
		return NULL;
	}
	element newMax = heap->copyElement(heap->array[0]);
	return newMax;
}

char *getHeapId(Heap heap){
	if (heap == NULL){
		return NULL;
	}
	char *heapNewName = (char *) malloc(strlen(heap->heapName)+1);
	if (heapNewName == NULL){
		return NULL;
	}
	strcpy(heapNewName, heap->heapName);
	return heapNewName;
}

int getHeapCurrentSize(Heap heap){
	if (heap == NULL){
		return -1;
	}
	return heap->currLen;
}
