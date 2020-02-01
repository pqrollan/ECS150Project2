#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct queue {
	/* TODO Phase 1 */
	queue_t next;
	int height;
	void* data;
	queue_t first;
};

//Does the result of queue_create count as an empty queue?
queue_t queue_create(void)
{
	printf("making space\n");
	queue_t q =  (queue_t)malloc(sizeof(struct queue));
	q->height = 0;
	q->data = NULL;
	q->first = NULL;
	q->next = NULL;
	printf("new queue created\n");
	return q;
}

int queue_destroy(queue_t queue){
	if (queue == NULL || queue_length(queue) > 0){
		return -1;
	}
	free(queue);
	queue = NULL;
	if (queue== NULL){
	// 	printf("working queue\n");
		printf("queue destroyed\n");

	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	printf("enqueue before test\n");
	if (queue == NULL || data == NULL){
		return -1;
	}
	printf("enqueue start \n");

	queue_t temp = queue_create();
	if (queue->height==0){
		printf("Queue's height is 0\n");
		queue->data = data;
		queue->height = 1;
		queue->first = queue;
		printf("first queue item assigned\n");
	}
	else{
		printf("Queue's height is %d\n", queue->height);
		printf("new queue height SHOULD BE %d\n", queue->height + 1);
		// temp = queue_create();
		if (temp == NULL){
			return -1;
		}
		printf("after temp made %d\n", queue->height + 1);
		temp->data = data;
		(*temp).height= (*queue).height;
		temp->first = queue->first;
		temp->next = NULL;
		queue->next = temp;
		(*queue) = (*temp);
		queue->height = queue->height+ 1;
		printf("the NEW Queue's height is %d\n", queue->height);
		
		printf("queue item added\n");
	}
	
	return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->height == 0){
		return -1;
	}
	if (queue->height == 1){
		*data = queue->data;
		queue->height = 0;
		queue->data = NULL;
		queue->first = NULL;
		queue->next = NULL;
	}
	else{
		queue_t temp = queue->first;
		printf("temp is assigned\n");
		queue->first = queue->first->next;
		printf("queue first is assigned\n");
		*data = temp->data;
		printf("data is assigned\n");
		queue->height = queue->height - 1;
		printf("height is reassigned\n");
		free(temp);
		temp = NULL;
	}
	printf("item dequeued\n");
	return 0;
	
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL){
		return -1;
	}
	queue_t temp= queue->first;
	if(temp->data == data){
		return queue_dequeue(queue, NULL);
	}
	for (int i = 0; i < queue_length(queue)-1; i++){ 
		if (temp->next->data == data){
			queue_t toDelete = temp->next;
			temp->next = temp->next->next;
			queue->height--;
			temp->height = queue->height;
			temp->first = queue->first;
			free(toDelete);
			printf("item deleted\n");
			return 0;
		}
		temp=temp->next;
	}
	return -1;
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || data == NULL){
		return -1;
	}

	queue_t temp= queue->first;
	for (int i = 0; i < queue_length(queue); i++){ 
		int returned = func(temp->data, arg);
		if (returned == 1){
			if (*data != NULL){
				*data = temp->data;
			}
			printf("item found, iteration stops\n");
			break;
		}
		temp=temp->next;
	}
	printf("iteration complete\n");
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL){
		return -1;
	}
	printf("height = %d\n", queue->height);
	printf("about to return height:\n");
	return (queue->height);
}

