#include <stdlib.h>
#include "Queue.h"

void QueueInit(Queue* pq)
{
	pq->head = (QNode*)malloc(sizeof(QNode));
	pq->head->next = NULL;
	pq->tail = pq->head;
}
int QIsEmpty(Queue* pq)
{
	return pq->head->next == NULL;
}

void Enqueue(Queue* pq, QData data)
{

	QNode* newNode = (QNode*)malloc(sizeof(QNode));
	newNode->data = data;

	newNode->next = NULL;
	pq->tail->next = newNode;
	pq->tail = newNode;
}
QData Dequeue(Queue* pq)
{
	if (QIsEmpty(pq))
		return -1;

	QNode* rnode = pq->head->next;
	QData rdata = rnode->data;

	pq->head->next = pq->head->next->next;

	free(rnode);

	if (QIsEmpty(pq))
		pq->tail = pq->head;

	return rdata;
}