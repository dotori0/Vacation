#pragma once

typedef int Data;

typedef struct _node
{
	Data data;
	struct _node* prev;
	struct _node* next;
}Node;

typedef struct _list
{
	int numOfData;
	Node* head;
	Node* tail;
	Node* cur;
}List;

void ListInit(List* plist);
void LInsert(List* plist, Data data);

int LFirst(List* plist, Data* pdata);
int LNext(List* plist, Data* pdata);

Data LRemove(List* plist);
int LCount(List* plist);