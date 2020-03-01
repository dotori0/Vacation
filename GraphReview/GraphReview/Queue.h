#pragma once

typedef int QData;

typedef struct _qnode
{
	QData data;
	struct _qnode* next;
}QNode;

typedef struct _queue
{
	QNode* head;
	QNode* tail;
}Queue;

void QueueInit(Queue* pq);
int QIsEmpty(Queue* pq);

void Enqueue(Queue* pq, QData data);
QData Dequeue(Queue* pq);