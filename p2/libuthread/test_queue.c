#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "queue.h"


//Unit test for queue_create()
void test_create(void)
{
    queue_t q;

    q = queue_create();
    assert(q != NULL);
}

//Tests enqueue and dequeue
void test_queue_simple(void)
{
	printf("Test queue simple\n");
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_length(q);
    queue_dequeue(q, (void**)&ptr);
    queue_length(q);
    assert(ptr == &data);
}

//Tests queue_destroy
void test_queue_destroy(){
	queue_t q;
	q = queue_create();
	int i = queue_destroy(q);

	assert(i == 0);
}

void test_enqueue_dequeue(void){
	printf(" \n \n \n Test enqueue dequeue \n \n \n");
	queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_length(q);
    queue_enqueue(q, &data);
    queue_length(q);
    queue_dequeue(q, (void**)&ptr);
    queue_length(q);
    queue_dequeue(q, (void**)&ptr);
    queue_length(q);
    queue_enqueue(q, &data);
    queue_length(q);
    queue_enqueue(q, &data);
    queue_length(q);
	queue_enqueue(q, &data);
	queue_length(q);
    queue_enqueue(q, &data);
    queue_length(q);
    queue_dequeue(q, (void**)&ptr);
    queue_length(q);
    queue_dequeue(q, (void**)&ptr);
    queue_length(q);


}


int main(){
	//test_create();
	test_queue_simple();
	//test_queue_destroy();
	test_enqueue_dequeue();

}

