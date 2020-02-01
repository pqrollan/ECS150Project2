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
    queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

//Tests queue_destroy
void test_queue_destroy(){
	queue_t q;
	q = queue_create();
	int i = queue_destroy(q);
	printf("Return value: %d\n", i);
	printf("%d", q->height);
	assert(q == NULL);
	assert(i == 0);


}


int main(){
	test_create();
	test_queue_simple();
	test_queue_destroy();

}

