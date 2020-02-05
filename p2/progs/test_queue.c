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
    assert(queue_length(q) == 0);
    assert(ptr == &data);
}

//Tests queue_destroy
void test_queue_destroy(){
	queue_t q;
	q = queue_create();
	int i = queue_destroy(q);

	assert(i == 0);
}

void test_height_dequeue_test(void){
	printf(" \n \n \n Test dequeue height\n \n \n");
	queue_t q;
    int data = 3, *ptr;

    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
	queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    queue_dequeue(q, (void**)&ptr);

    assert(queue_length(q) == 2);
}

void test_height_delete_test(void){
    printf(" \n \n \n Test delete height\n");
    queue_t q;
    int data = 3;

    q = queue_create();
    assert(queue_delete(q, &data)==-1);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_delete(q, &data);
    queue_delete(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_enqueue(q, &data);
    queue_delete(q, &data);
    queue_delete(q, &data);

    assert(queue_length(q) == 2);
}


void test_simple_delete(void)
{
    printf("Test delete simple\n");
    queue_t q;
    int data = 3;
    int dataValid = 2;


    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &dataValid);
    queue_enqueue(q, &data);
    queue_delete(q,&dataValid);
    /*
        testing that 1 was deleted and that there exists no other datavalid
    */
    assert(queue_length(q) == 2);
    assert(queue_delete(q,&dataValid)==-1);

    queue_delete(q,&data);
    queue_delete(q,&data);
    /*
        testing that we can successfully delete the last 2 items
    */
    assert(queue_length(q) == 0);

}

void test_delete_first_element(void){
    printf("Test delete first element\n");
    queue_t q;
    int dataValid = 3;
    int data = 2, *ptr;


    q = queue_create();
    queue_enqueue(q, &dataValid);
    queue_enqueue(q, &data);
    queue_delete(q,&dataValid);
    
    //Test height of queue after delete.
    assert(queue_length(q) == 1);
    assert(queue_delete(q,&dataValid)==-1); //Test whether dataValid still exists in the queue
    
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data); //Test whether the remaining element is the correct one.

    assert(queue_delete(q, &data) == -1); //Test whether data is still in the queue.
}


void test_delete_last_element(void){
    printf("Test delete last element\n");
    queue_t q;
    int dataValid = 3;
    int data = 2, *ptr;


    q = queue_create();
    queue_enqueue(q, &data);
    queue_enqueue(q, &dataValid);
    queue_delete(q,&dataValid);
    
    //Test height of queue after delete.
    assert(queue_length(q) == 1);
    assert(queue_delete(q,&dataValid)==-1); //Test whether dataValid still exists in the queue
    
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data); //Test whether the remaining element is the correct one.

    assert(queue_delete(q, &data) == -1); //Test whether data is still in the queue.
}

/* Callback function that increments items by a certain value */
static int inc_item(void *data, void *arg)
{
    int *a = (int*)data;
    int inc = (int)(long)arg;

    *a += inc;

    return 0;
}

/* Callback function that finds a certain item according to its value */
static int find_item(void *data, void *arg)
{
    int *a = (int*)data;
    int match = (int)(long)arg;

    if (*a == match)
        return 1;

    return 0;
}

void test_iterator(void)
{
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)(sizeof(data) / sizeof(data[0])); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, inc_item, (void*)1, NULL);
    assert(data[0] == 2);

    /* Find and get the item which is equal to value '5' */
    ptr = NULL;
    queue_iterate(q, find_item, (void*)5, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 5);
    assert(ptr == &data[3]);
}



int main(){
	//test_create();
	test_queue_simple();
	//test_queue_destroy();
	test_height_dequeue_test();
    test_height_delete_test();

    test_simple_delete();
    test_delete_first_element();
    test_delete_last_element();
    test_iterator();

}

