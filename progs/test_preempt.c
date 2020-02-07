/*
 * This is a Thread test file. 
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
#include <assert.h>


int f(void* arg)
{
	printf("thread%d\n", uthread_self());
	return 0;
}

int chain_test2(void* arg)
{
	uthread_t temp = uthread_create(f, NULL);
	int retval = 10;
	uthread_join(temp,&retval);
	printf("return value is: %d\n", retval);
	return -3;
}

int chain_test1(void* arg)
{
	uthread_t temp = uthread_create(chain_test2, NULL);
	int retval = 10;
	uthread_join(temp,&retval);
	printf("return value is: %d\n", retval);
	return -5;
}


int join_main(void* arg)
{
	assert(uthread_join(0, NULL)==-1);
	return -2;
}

int join_self(void* arg)
{
	assert(uthread_join(uthread_self(), NULL)==-1);
	return -2;
}

int join_inf(void* arg)
{
	assert(uthread_join(800, NULL)==-1);
	return -2;
}

int join_same(void* arg)
{
	if(uthread_join(*((uthread_t*) arg), NULL)==-1){
		return -2;
	}
	else{
		return 0;
	}
	
}

int yieldWithOnlyOneThread(void* arg){
	uthread_yield();
	uthread_yield();
	return -1;
}

/*
	testing the preempt signal
*/
int* while_global_condition = NULL;


int while_thread(void* arg){
	while (*while_global_condition != 1){
		printf("GC = %d\n", *while_global_condition);
	}
	printf("end of while\n");
	return 0;
}

int while_end_thread(void * arg){
	printf("reached tier 2\n");
	*while_global_condition = 1;
	return 0;
}

int main(void)
{
	while_global_condition= (int*) malloc(sizeof(int));
	*while_global_condition = 0;

	/*
		This tests creates a while loop which only finishes when a global
	is set = to 1. This global is set to 2 in thread 2 which is reached
	when preempt is called on thread 1. Using print statement, the while
	in thread one will check the actual value of the global rather than
	its apparently cashed value, and will break. Without the print, the
	program hangs. 
	*/ 
	uthread_t tid_2 = uthread_create(while_thread, NULL);
	uthread_t tid_1 = uthread_create(while_end_thread, NULL);
	int retval = 0;
	uthread_join(tid_2, &retval);
	assert(retval == 0);
	printf("^^ preemp test case ^^\n\n");

	/*
		Test shows that main will wait till all ready's are finished
	*/
	tid_1 = uthread_create(f, NULL);
	tid_2 = uthread_create(f, NULL);
	uthread_join(tid_1, NULL);
	printf("thread 3 and 4 ran, join of thread 3 completed, join 4 initiated\n");
	uthread_join(tid_2, NULL);


	/*
		Test for creating, closing, and recreating a thread
	*/ 
	tid_1 = uthread_create(f, NULL);
	uthread_join(tid_1, NULL);
	printf("thread 5 created and join, join 6 create initiated\n");
	tid_2 = uthread_create(f, NULL);
	uthread_join(tid_2, NULL);

	/*
		test to join main successfully fails
	*/
	printf("\njoin main/self/ and invalid tests:\n");
	tid_1 = uthread_create(join_main, NULL);
	retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	/*
		test to join self successfully fails
	*/
	tid_1 = uthread_create(join_self, NULL);
	retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	/*
		test to join inf successfully fails
	*/
	tid_1 = uthread_create(join_inf, NULL);
	retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	/*
		test for two threads to join the same thread successfully fails
	*/
	printf("joining a joined thread test:\n");
	uthread_t tid_0 = uthread_create(f, NULL);
	tid_1 = uthread_create(join_same, &tid_0);
	tid_2 = uthread_create(join_same, &tid_0);
	retval = 0;
	uthread_join(tid_2, &retval);
	assert(retval == -2);

	/* 
		A thread that yeilds to no one will simply begin again
	*/
	printf("thread yeilds to no other:\n");
	tid_2 = uthread_create(yieldWithOnlyOneThread, NULL);
	retval = 0;
	uthread_join(tid_2, &retval);
	assert(retval == -1);

	/* 
		a chain of threads joining will correctly exit() then unblock in 
		order
	*/
	printf("multi join chain test: \n");
	retval = 0;
	tid_0 = uthread_create(chain_test1, NULL);
	uthread_join(tid_0, &retval);
	printf("final retval is: %d\n", retval);
	printf("return values in order: 0 -3 -5\n");
	

	printf("Passes tests\n");

	

	return 0;
}