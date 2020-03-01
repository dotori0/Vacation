#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "DFS.h"
#include "Stack.h"

#define TRUE 1
#define FALSE 0

void DFShowGraphVertex(ALGraph* pg, int startV)
{
	Stack stack;
	int visitV = startV;
	int nextV;

	StackInit(&stack);
	VisitVertex(pg, visitV);
	SPush(&stack, visitV);

	while (LFirst(&pg->adjList[visitV], &nextV) == TRUE)
	{
		int visitFlag = FALSE;

		if (VisitVertex(pg, nextV) == TRUE)
		{
			SPush(&stack, visitV);
			visitV = nextV;
			visitFlag = TRUE;
		}
		else
		{
			while (LNext(&pg->adjList[visitV], &nextV) == TRUE)
			{
				if (VisitVertex(pg, nextV) == TRUE)
				{
					SPush(&stack, visitV);
					visitV = nextV;
					visitFlag = TRUE;
					break;
				}
			}
		}

		if (visitFlag == FALSE)
		{
			if (SIsEmpty(&stack) == TRUE)
			{
				break;
			}
			else
			{
				visitV = SPop(&stack);
			}
		}
	}

	printf("\n");

	memset(pg->visitInfo, 0, sizeof(int) * pg->numV);
}