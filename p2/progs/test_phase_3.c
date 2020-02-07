

/*
 * Simple hello world test
 *
 * Tests the creation of a single thread and its successful return.
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

int while_thread(void* arg){
	int i =0;
	while(i < 10000){
		printf("looping %d\n",i);
		i++;
	}
	return 0;
}

int main(void)
{

	uthread_t tid_1 = uthread_create(f, NULL);
	uthread_t tid_2 = uthread_create(f, NULL);
	uthread_join(tid_1, NULL);
	printf("join 1 complete, join 2 initiated\n");
	uthread_join(tid_2, NULL);

	printf("\n\n test 2:\n");
	tid_1 = uthread_create(f, NULL);
	uthread_join(tid_1, NULL);
	printf("join 1 complete, join 2 initiated\n");
	tid_2 = uthread_create(f, NULL);
	uthread_join(tid_2, NULL);

	tid_1 = uthread_create(join_main, NULL);
	int retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	tid_1 = uthread_create(join_self, NULL);
	retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	tid_1 = uthread_create(join_inf, NULL);
	retval = 0;
	uthread_join(tid_1, &retval);
	assert(retval == -2);

	uthread_t tid_0 = uthread_create(f, NULL);
	tid_1 = uthread_create(join_same, &tid_0);
	tid_2 = uthread_create(join_same, &tid_0);
	retval = 0;
	uthread_join(tid_2, &retval);
	assert(retval == -2);

	tid_2 = uthread_create(yieldWithOnlyOneThread, NULL);
	retval = 0;
	uthread_join(tid_2, &retval);
	assert(retval == -1);

	printf("\n\nmulti join test: \n");
	retval = 0;
	tid_0 = uthread_create(chain_test1, NULL);
	uthread_join(tid_0, &retval);
	printf("final t0 retval is: %d\n", retval);
	
	
	tid_2 = uthread_create(while_thread, NULL);
	uthread_join(tid_2, &retval);
	

	printf("Passes tests\n");

	

	return 0;
}