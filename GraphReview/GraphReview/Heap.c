#include <stdlib.h>
#include "Heap.h"

#define PARENT(IDX) IDX / 2
#define LEFT(IDX) IDX * 2
#define RIGHT(IDX) IDX * 2 + 1

void HeapInit(Heap* ph, PriorityComp* pc)
{
	ph->numOfData = 0;
	ph->comp = pc;
}
int HIsEmpty(Heap* ph)
{
	return ph->numOfData == 0;
}

int GetHiPriChildIdx(Heap* ph, int idx)
{
	if (LEFT(idx) > ph->numOfData)
		return 0;
	else if (LEFT(idx) == ph->numOfData)
		return LEFT(idx);
	else
	{
		if (ph->heapArr[LEFT(idx)].pr < ph->heapArr[RIGHT(idx)].pr)
			return LEFT(idx);
		else
			return RIGHT(idx);
	}
}

void HInsert(Heap* ph, HData data, Priority pr)
{
	HeapElem newElem;
	newElem.data = data;
	newElem.pr = pr;

	int idx = ph->numOfData + 1;

	while (idx != 1)
	{
		if (pr < ph->heapArr[PARENT(idx)].pr)
		{
			ph->heapArr[idx] = ph->heapArr[PARENT(idx)];
			idx = PARENT(idx);
		}
		else
			break;
	}

	ph->heapArr[idx] = newElem;
	ph->numOfData += 1;
}
HData HDelete(Heap* ph)
{
	HData rdata = ph->heapArr[1].data;
	HeapElem LElem = ph->heapArr[ph->numOfData];

	int parentIdx = 1;
	int childIdx;

	while (childIdx = GetHiPriChildIdx(ph, parentIdx))
	{
		if (LElem.pr <= ph->heapArr[childIdx].pr)
			break;
		ph->heapArr[parentIdx] = ph->heapArr[childIdx];
		parentIdx = childIdx;
	}

	ph->heapArr[parentIdx] = LElem;
	ph->numOfData -= 1;
	return rdata;
}