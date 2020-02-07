#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "queue.h"

/*
	Struct representing an element of a queue. 
	Contains data and pointers to the previous and next nodes
*/
struct node {
	void* data;
	struct node* prev;
	struct node* next;
};

/*
	Struct representing a queue. A queue is implemented as a doubly linked list.
	A queue has pointers to its first and last elements, and stores its height
*/
struct queue {
	struct node* first;
	struct node* last;
	int height;
};




queue_t queue_create(void)
{
	//If the queue was successfully allocated, populate its fields.
	queue_t q =  (queue_t)malloc(sizeof(struct queue)); 
	if(q != NULL){
		q->height = 0;
		q->first = NULL;
		q->last = NULL;
	}
	return q;
}

int queue_destroy(queue_t queue){
	if (queue == NULL || queue_length(queue) > 0){
		return -1;
	}
	free(queue); //Deallocate the queue
	queue = NULL;
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	
	if (queue == NULL || data == NULL){
		return -1;
	}
	
	struct node* new_node = (struct node*) malloc(sizeof(struct node)); //Allocate space for the new node
	new_node->data = data; 
	//Link the new node to the end of the queue
	new_node->prev = queue->last;
	new_node->next = NULL;
	queue->last = new_node;
	queue->height = queue->height+ 1;
	//If the queue has one element, the new node will be its first node. Otherwise link the end of the queue to the new node.
	if (queue->height == 1){
		queue->first = new_node;
	}
	else{
		new_node->prev->next = new_node;
	}
	
	return 0;

}

int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || data == NULL || queue->height == 0){
		return -1;
	}

	struct node* dequeued_node = queue->first;
	*data = dequeued_node->data; //Extract the dequeued node's data
	//Unlink the node from the queue
	queue->first = dequeued_node->next;
	queue->height = queue->height -1;

	//If the queue is not empty, the first element's previous node will be null. Else, if the queue is empty, set its last element to null.
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
	struct node* deleted_node= queue->first;

	//Iterate through the queue searching for the desired node.
	for (int i = 0; i < queue_length(queue); i++){ 
		if (deleted_node->data == data){
			//If the desired node is found, unlink it from the queue.
			if(i!=0){
				deleted_node->prev->next = deleted_node->next;
			}
			else{
				queue->first= deleted_node->next;
			}
			if ((i+1) != queue_length(queue)){
				deleted_node->next->prev = deleted_node->prev;
			}
			else{
				queue->last= deleted_node->prev;
			}
			queue->height--;
			free (deleted_node); //Free the deleted node once unlinked and return
			return 0;
		}
		deleted_node=deleted_node->next;
	}
	return -1;
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	if (queue == NULL || func == NULL){
		return -1;
	}

	//Iterate through the queue, calling the function on every element. Collect the return value. If it is 1, return from the function.
	struct node* cur_node = queue->first;
	for (int i = 0; i < queue_length(queue); i++){ 
		int returned = func(cur_node->data, arg);
		if (returned == 1){
			if (data != NULL){
				*data = cur_node->data;
			}
			break;
		}
		cur_node = cur_node->next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if (queue == NULL){
		return -1;
	}
	return (queue->height);
}

