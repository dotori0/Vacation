#pragma once

typedef int SData;

typedef struct _snode
{
	SData data;
	struct _snode* next;
}SNode;

typedef struct _stack
{
	SNode* head;
}Stack;

void StackInit(Stack* pstack);
int SIsEmpty(Stack* pstack);
void SPush(Stack* pstack, SData data);
SData SPop(Stack* pstack);
SData SPeek(Stack* pstack);