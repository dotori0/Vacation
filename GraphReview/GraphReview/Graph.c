#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include "Graph.h"

#define TRUE 1
#define FALSE 0

int VisitVertex(ALGraph* pg, int visitV)
{
	if (pg->visitInfo[visitV] == 0)
	{
		pg->visitInfo[visitV] = 1;
		printf("%c ", visitV + 'A');
		return TRUE;
	}
	return FALSE;
}

void GraphInit(ALGraph* pg, int nv)
{
	int i;
	pg->numE = 0;
	pg->numV = nv;

	pg->adjList = (List*)malloc(sizeof(List) * nv);

	for (i = 0; i < nv; i++)
	{
		ListInit(&pg->adjList[i]);
	}

	pg->visitInfo = (int*)malloc(sizeof(int) * nv);
	memset(pg->visitInfo, 0, sizeof(int) * nv);
}
void GraphDestroy(ALGraph* pg)
{
	if (pg->adjList != NULL)
		free(pg->adjList);
	if (pg->visitInfo != NULL)
		free(pg->visitInfo);
}
void AddEdge(ALGraph* pg, int fromV, int toV)
{
	LInsert(&pg->adjList[fromV], toV);
	LInsert(&pg->adjList[toV], fromV);
	pg->numE += 1;
}
void ShowGraphEdgeInfo(ALGraph* pg)
{
	int i;
	int vx;

	for (i = 0; i < pg->numV; i++)
	{
		printf("%c -> ", i + 'A');
		if (LFirst(&pg->adjList[i], &vx))
		{
			printf("%c ", vx + 'A');
			while (LNext(&pg->adjList[i], &vx))
			{
				printf("%c ", vx + 'A');
			}
		}
		printf("\n");
	}
}