#pragma once

#define HEAP_LEN 100

typedef char HData;
typedef int Priority;

typedef int PriorityComp(HData, HData);

typedef struct _headElem
{
	HData data;
	Priority pr;
}HeapElem;

typedef struct _heap
{
	int numOfData;
	HeapElem heapArr[HEAP_LEN];
	PriorityComp* comp;
}Heap;

void HeapInit(Heap* ph);
int HIsEmpty(Heap* ph);

void HInsert(Heap* ph, HData data, Priority pr);
HData HDelete(Heap* ph);