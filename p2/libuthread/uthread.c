
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
	uthread_ctx_t context;
	enum uthread_State state;
	int retval;

};

struct TCB runningThread;

struct TCB* tbcArray[USHRT_MAX]= {NULL};

/* TODO Phase 2 */


void uthread_yield(void)
{
	struct TCB* temp;
	queue_enqueue(readyQueue, &runningThread); //Enqueue the running thread to the ready queue
	queue_dequeue(readyQueue, (void**) &temp); //Get the next thread ready to run
	uthread_ctx_switch(&runningThread.context, &temp->context); //Switch the contexts
	runningThread = (*temp); //Switch the running thread
}

uthread_t uthread_self(void)
{
	return runningThread.tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	if(tid_count < USHRT_MAX){
		//If main:
		uthread_ctx_t c;
		
		struct TCB* t = (struct TCB*) malloc (sizeof(struct TCB));

		void* s = uthread_ctx_alloc_stack();
		if(tid_count == 0){
			uthread_ctx_switch(&c, &c);
			*t = {0, s, c, RUNNING, -1};
			runningThread = *t;
			readyQueue = queue_create();
			blockedQueue = queue_create();
			tbcArray[tid_count]= t;
			tid_count++;
			return runningThread.tid;
		}
		else{
			uthread_ctx_init(&c, s, func, arg);
			*t = {tid_count, s, c, READY, -1};
			queue_enqueue(readyQueue, &t);
			tbcArray[tid_count]= t;
			tid_count++;
			return t->tid;
		}
	}
	return -1;
}

void uthread_exit(int retval)
{
	struct TCB* temp;
	//Set the running thread's return value to retval
	runningThread.retval = retval;
	//Set the status of the thread to terminated
	runningThread.state = ZOMBIE;
	//Switch to the next ready thread
	queue_dequeue(readyQueue, (void**) &temp); //Get the next thread ready to run
	uthread_ctx_switch(&runningThread.context, &temp->context); //Switch the contexts
	runningThread = (*temp); //Switch the running thread

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

 	*retval = 1;

 	while(1 == 1){
 		if (queue_length(readyQueue) == 0){
 			break;
 		}
 		else{
 			uthread_yield();
 			/*
				get tid, get retval, free tcb with tid 
 			*/
 		}
 	}
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

