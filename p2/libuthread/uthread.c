
#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <limits.h>

#include "context.h"
#include "preempt.h"
#include "queue.h"
#include "uthread.h"


enum uthread_State {RUNNING, READY, ZOMBIE, BLOCKED};

int tid_count=0;
queue_t readyQueue;
queue_t blockedQueue;
// queue_t zombies




struct TCB {
	uthread_t tid;
	void* stack;
	uthread_ctx_t* context;
	enum uthread_State state;
	int retval;

};

struct TCB* runningThread;

struct TCB* tcbArray[USHRT_MAX]= {NULL};

/* TODO Phase 2 */


void uthread_yield(void)
{
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	//struct TCB* curr_context = (struct TCB*) malloc (sizeof(struct TCB));
	uthread_t runningThreadTid = runningThread->tid;

	if (runningThread){
		printf("Running thread id = %hu\n", runningThread->tid);
	}
	queue_enqueue(readyQueue, (void *)tcbArray[runningThreadTid]); //Enqueue the running thread to the ready queue
	queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
	printf("enqueue dequeue success\n");
	runningThread = next_thread;
	printf("Running thread id = %hu\n", runningThread->tid);
	printf("entering context switch\n");
	
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); //Switch the contexts
	 //Switch the running thread
}

static void uthread_join_yield(void)
{
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	//struct TCB* curr_context = (struct TCB*) malloc (sizeof(struct TCB));
	uthread_t runningThreadTid = runningThread->tid;

	if (runningThread){
		printf("Running thread id = %hu\n", runningThread->tid);
	}
	queue_enqueue(blockedQueue, (void *)tcbArray[runningThreadTid]); //Enqueue the running thread to the ready queue
	queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
	printf("enqueue dequeue success\n");
	runningThread = next_thread;
	printf("Running thread id = %hu\n", runningThread->tid);
	printf("entering context switch\n");
	
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); //Switch the contexts
	 //Switch the running thread
}

uthread_t uthread_self(void)
{
	return runningThread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(tid_count < USHRT_MAX){
		//If main:
		printf("Create_thread start\n");
		uthread_ctx_t* c = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
		void* s = uthread_ctx_alloc_stack();
		struct TCB* t = (struct TCB*) malloc (sizeof(struct TCB));

		
		if(tid_count == 0){
			printf("Creating main thread\n");
			uthread_ctx_t* c_main = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
			void* s_main = uthread_ctx_alloc_stack();
			runningThread = (struct TCB*) malloc (sizeof(struct TCB));
			printf("space allocated in main\n");
			uthread_ctx_switch(c_main, c_main);
			*runningThread = (struct TCB){0, s_main, c_main, RUNNING, -1};

			readyQueue = queue_create();
			blockedQueue = queue_create();
			tcbArray[tid_count]= runningThread;
			tid_count++;
		}

		printf("Creating a new thread\n");
		uthread_ctx_init(c, s, func, arg);
		struct TCB temp= {tid_count, s, c, READY, -1};
		*t = temp;
		queue_enqueue(readyQueue, t);
		tcbArray[tid_count]= t;
		tid_count++;
		return t->tid;
	
	}
	return -1;
}

void uthread_exit(int retval)
{
	printf("Entered uthread_exit\n");
	struct TCB* next_thread;
	//Set the runningg thread's return value to retval
	runningThread->retval = retval;
	//Set the status of the thread to terminated
	runningThread->state = ZOMBIE;
	//Switch to the next ready thread
	uthread_t runningThreadTid = runningThread->tid;

	if (queue_length(readyQueue)>0){
		printf("switching to ready thread \n");
		queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
		runningThread = next_thread; //Switch the running thread
		uthread_ctx_switch(tcbArray[runningThreadTid]->context, runningThread->context); //Switch the contexts
		
	}
	else{
		printf("No more ready threads to switch to\n");
		runningThread = tcbArray[0]; 
		uthread_ctx_switch(tcbArray[runningThreadTid]->context, runningThread->context);
	}
	
}


int uthread_join(uthread_t tid, int *retval)
{
	/* Phase 2 */
 	//while true:
 	//	if the ready queue is empty, break
 	//	else, call uthread_yield

 	//At end, call queue_destroy

	if (tid == 0){
		return -1;
	}
	if (retval){
		printf("retval not null\n");
	}

 	while(1 == 1){
 		if (queue_length(readyQueue) == 0){
 			printf("Ready to break out\n");
 			break;
 		}
 		else{
			printf("We are yeilding\n");
 			uthread_yield();
 			/*
				get tid, get retval, free tcb with tid 
 			*/
 		}
 	}
 	printf("attempting to destroy\n");
 	queue_destroy(readyQueue);


 	/*
 		if tcbArray[tid].state == ZOMBIE:
			retval = tcbArray[tid]->retval
			deallac stack 
			dealloc context 
			free (tcbArray[tid])

 		if tcbArray[tid].state == READY:
 			Enqueue the running thread to the blocked queue
 			Context switch to T2.


 		if T2 is dead:
 	*/
	
 	/* TODO Phase 3 */
 	return 0;
}

