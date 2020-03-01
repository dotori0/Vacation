#include <stdlib.h>
#include "Stack.h"

void StackInit(Stack* pstack)
{
	pstack->head = (SNode*)malloc(sizeof(SNode));
	pstack->head->next = NULL;
}
int SIsEmpty(Stack* pstack)
{
	return pstack->head->next == NULL;
}
void SPush(Stack* pstack, SData data)
{
	SNode* newNode = (SNode*)malloc(sizeof(SNode));
	newNode->data = data;
	
	newNode->next = pstack->head->next;
	pstack->head->next = newNode;
}
SData SPop(Stack* pstack)
{
	SNode* rnode = pstack->head->next;
	SData rdata = rnode->data;

	pstack->head->next = pstack->head->next->next;

	free(rnode);
	return rdata;
}
SData SPeek(Stack* pstack)
{
	return pstack->head->next;
}