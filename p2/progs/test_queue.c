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
    assert(queue_length(q) == 0);
    assert(ptr == &data);
}

//Tests whether a queue pointer can be created and destroyed multiple times
void test_multi_create_destroy(void)
{
    queue_t q;

    q = queue_create();
    assert(q != NULL);
    queue_destroy(q);

    q = queue_create();
    assert(q != NULL);
    queue_destroy(q);
	assert(queue_length(q) == 0);
}

//Ensure that a null queue can't be destroyed
void test_destroy_NULL(void)
{
    queue_t q = NULL;

    int i = queue_destroy(q);
	assert(i == -1);
}

//Ensure that a populated queue can't be destroyed
void test_destroy_filled(void)
{
    queue_t q = NULL;
    int dataValid = 2;
    q = queue_create();
    queue_enqueue(q, &dataValid);
    int i = queue_destroy(q);
	assert(i == -1);
}

//Perform a series of enqueues and dequeues, and ensure that the height is correct
void test_height_dequeue_test(void){
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

//Ensure that after multiple enqueues and deletes, the height is correct
void test_height_delete_test(void){
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

//Tests that deletes returns the correct value and removes the element from the queue
void test_simple_delete(void)
{
    queue_t q;
    int dataValid = 2;


    q = queue_create();
    queue_enqueue(q, &dataValid);
    queue_delete(q,&dataValid);
    /*
        testing that 1 was deleted and that there exists no other datavalid
    */
    assert(queue_length(q) == 0);
    assert(queue_delete(q,&dataValid)==-1);
    assert(queue_length(q) == 0);

}

//Test multiple deletes
void test_embedded_delete(void)
{
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

//Tests that first element can be deleted successfully
void test_delete_first_element(void){
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
    assert(ptr == &data); 
    //Test whether the remaining element is the correct one.

    assert(queue_delete(q, &data) == -1); 
    //Test whether data is still in the queue.
}

//Ensures that last element can be deleted successfully
void test_delete_last_element(void){
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

//Ensure that the iterate function is applied to eachelement of the queue
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

// Ensure that after delete alters elements of the queue, they can be iterated through successfully
void test_iterator_with_delete(void)
{
    queue_t q;
    int data[] = {1, 2, 1, 2, 1};
    int i;
    int *ptr;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < (int)(sizeof(data) / sizeof(data[0])); i++)
        queue_enqueue(q, &data[i]);

    /* Add value '1' to every item of the queue */
    queue_iterate(q, find_item, (void*)2, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 2);
    queue_delete(q, &data[2]);

    queue_iterate(q, find_item, (void*)2, (void**)&ptr);
    assert(ptr != NULL);
    assert(*ptr == 2);
    queue_delete(q, &data[2]);

    assert(queue_delete(q, &data[2])==-1);
}

//Test to enqueue to a null queue, fails successfully
void test_enqueue_nullqueue(void)
{
    queue_t q;
    int data = 3;
    q = NULL;
    assert(queue_enqueue(q, &data)==-1);
}

//Test to enqueue to a null element to a queue, fails successfully
void test_enqueue_nodata(void)
{
    queue_t q;
    int* data = NULL;

    q = queue_create();
    assert(queue_enqueue(q, data)==-1);
}

//Test to dequeue to a null queue, fails successfully
void test_dequeue_nullqueue(void)
{
    queue_t q;
    int data = 3;
    int* data_ptr = &data;
    q = NULL;
    assert(queue_dequeue(q, (void**) &data_ptr)==-1);
}

//Test to dequeue without a pointer to assign it's data to
void test_dequeue_nodata(void)
{
    queue_t q;
    int data = 3;
    q = queue_create();
    queue_enqueue(q, &data);
    assert(queue_dequeue(q, NULL)==-1);
}

//Ensure that an empty queue can't be dequeued
void test_dequeue_emptyqueue(void)
{
    queue_t q;
    int data = 3;
    int* data_ptr = &data;
    q = queue_create();
    assert(queue_dequeue(q, (void**) &data_ptr)==-1);
}



int main(){
	test_create();
    test_multi_create_destroy();

    test_destroy_NULL();
    test_destroy_filled();

	test_queue_simple();
	test_height_dequeue_test();
    test_height_delete_test();

    test_simple_delete();
    test_embedded_delete();
    test_delete_first_element();
    test_delete_last_element();

    test_iterator();
    test_iterator_with_delete();

    test_enqueue_nullqueue();
    test_enqueue_nodata();

    test_dequeue_nullqueue();
    test_dequeue_nodata();
    test_dequeue_emptyqueue();
}

