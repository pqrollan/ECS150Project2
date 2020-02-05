#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

struct node {
	void* data;
	struct node* prev;
	struct node* next;
};

struct queue {
	/* TODO Phase 1 */
	struct node* first;
	struct node* last;
	int height;
};



//Does the result of queue_create count as an empty queue?
queue_t queue_create(void)
{
	//printf("making space\n");
	queue_t q =  (queue_t)malloc(sizeof(struct queue));
	q->height = 0;
	q->first = NULL;
	q->last = NULL;
	//printf("new queue created\n");
	return q;
}

int queue_destroy(queue_t queue){
	if (queue == NULL || queue_length(queue) > 0){
		return -1;
	}
	free(queue);
	queue = NULL;
	if (queue== NULL){
		printf("queue destroyed\n");
	}
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	
	if (queue == NULL || data == NULL){
		return -1;
	}
	
	struct node* temp = (struct node*) malloc(sizeof(struct node));
	printf("space made in enqueue\n");
	temp->data = data;
	temp->prev = queue->last;
	temp->next = NULL;
	queue->last = temp;
	queue->height = queue->height+ 1;
	if (queue->height == 1){
		queue->first = temp;
	}
	else{
		temp->prev->next = temp;
	}
	printf("node added to queue\n");
	
	return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->height == 0){
		return -1;
	}
	
	struct node* temp = queue->first;
	*data = temp->data;
	queue->first = temp->next;
	queue->height = queue->height -1;

	if (queue->height > 0){
		queue->first->prev = NULL;
	}
	else{
		queue->last= NULL;
	}

	return 0;
	
}

int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL || queue_length(queue) == 0){
		return -1;
	}
	struct node* temp= queue->first;

	for (int i = 0; i < queue_length(queue); i++){ 
		if (temp->data == data){
			if(i!=0){
				temp->prev->next = temp->next;
			}
			else{
				queue->first= temp->next;
			}
			if ((i+1) != queue_length(queue)){
				temp->next->prev = temp->prev;
			}
			else{
				queue->last= temp->prev;
			}
			queue->height--;
			free (temp);
			return 0;
		}
		temp=temp->next;
	}
	return -1;
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func == NULL){
		return -1;
	}

	struct node* temp = queue->first;
	for (int i = 0; i < queue_length(queue); i++){ 
		int returned = func(temp->data, arg);
		if (returned == 1){
			if (data != NULL){
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
	return (queue->height);
}

