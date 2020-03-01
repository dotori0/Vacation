#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "BFS.h"
#include "Queue.h"

#define TRUE 1
#define FALSE 0

void BFShowGraphVertex(ALGraph* pg, int startV)
{
	int visitV = startV;
	int nextV;
	VisitVertex(pg, visitV);
	Queue queue;
	QueueInit(&queue);

	while (TRUE)
	{
		int visitFlag = FALSE;

		LFirst(&pg->adjList[visitV], &nextV);

		if (VisitVertex(pg, nextV))
		{
			Enqueue(&queue, nextV);
			visitFlag = TRUE;
		}
		while (LNext(&pg->adjList[visitV], &nextV))
		{
			if (VisitVertex(pg, nextV))
			{
				Enqueue(&queue, nextV);
				visitFlag = TRUE;
			}
		}
		if (QIsEmpty(&queue))
			break;
		else
		{
			visitV = Dequeue(&queue);
		}
	}

	printf("\n");

	memset(pg->visitInfo, 0, sizeof(int) * pg->numV);
}