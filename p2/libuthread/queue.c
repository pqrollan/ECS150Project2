#include <stdint.h>
#include <stdlib.h>
#include <string.h>

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
	queue_t q =  (queue_t)malloc(sizeof(queue));
	q->height = 0;
	q->data = NULL;
	q->first = NULL;
	q->next = NULL;
	return q;
}

int queue_destroy(queue_t queue)
{
	queue = NULL;
}

int queue_enqueue(queue_t queue, void *data)
{
	/* TODO Phase 1 */
	queue_t temp = queue_create();
	temp->data = data
	queue->next = temp
	temp->height= queue->height++;
	temp->first = queue->first;
	temp->next = NULL;
	queue = temp;

}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
	if queue->first != NULL{
		temp = queue->first;
		queue->first = temp->next;
		*data = temp->data
		queue->height--;
		free(temp);
		temp = NULL;
	}

	
}

int queue_delete(queue_t queue, void *data)
{
	queue_t temp= queue->first;
	for (int i = 0; i< queue.length(); i++){ 
		
		i--;
		
	}
	
}

int queue_iterate(queue_t queue, queue_func_t func, void *arg, void **data)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
	return queue->length;
}

