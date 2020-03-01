#include <stdio.h>
#include <stdlib.h>
#include "BFS.h"
#include "DFS.h"

void Show(ALGraph graph, char startV)
{
	printf("\n[ ���� ]\n");
	ShowGraphEdgeInfo(&graph);

	printf("\n[ Ž�� ]\n");
	printf("BFS : ");
	BFShowGraphVertex(&graph, startV);

	printf("DFS : ");
	DFShowGraphVertex(&graph, startV);

	GraphDestroy(&graph);
}

int main(void)
{	
	ALGraph graph;
	int numV;
	char startV;
	char toV;
	char input[50];

	printf("������ �� : ");
	scanf("%d", &numV);

	GraphInit(&graph, numV);

	fgets(input, 50, stdin);
	while (1)
	{
		fgets(input, 50, stdin);
		startV = input[0];
		toV = input[2];

		if (startV == '0')
			break;

		AddEdge(&graph, startV - 'A', toV - 'A');
	}
	
	printf("Ž�� ������ : ");
	fgets(input, 50, stdin);
	startV = input[0] - 'A';

	Show(graph, startV);

	return 0;
}