
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


enum uthread_State {RUNNING, READY, ZOMBIE};

int tid_count=0;
queue_t readyQueue;
queue_t blockedQueue;



struct TCB {
	uthread_t tid;
	void* stack;
	uthread_ctx_t context;
	enum uthread_State state;

};

struct TCB runningThread;

/* TODO Phase 2 */

void uthread_yield(void)
{
	/*
	uthread_ctx_t c;
	void* s = NULL;
	struct TCB temp= {5, s, c, RUNNING };
	struct TCB *temp_point= &temp;
	*/
	struct TCB* temp;
	queue_dequeue(readyQueue, (void**) &temp);
	queue_enqueue(readyQueue, &runningThread);
	uthread_ctx_switch(&runningThread.context, &temp->context);
	runningThread = (*temp); 
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
		void* s = uthread_ctx_alloc_stack();
		if(tid_count == 0){
			uthread_ctx_switch(&c, &c);
			struct TCB t = {0, s, c, RUNNING };
			runningThread = t;
			readyQueue = queue_create();
			blockedQueue = queue_create();
			return runningThread.tid;
		}
		else{
			uthread_ctx_init(&c, s, func, arg);
			struct TCB thread = {tid_count++, s, c, READY };
			queue_enqueue(readyQueue, &thread);
			return thread.tid;
		}
	}
	return -1;
	
}

void uthread_exit(int retval)
{
	/* TODO Phase 2 */
	retval = 0;
	retval = retval + 1;
}


// int uthread_join(uthread_t tid, int *retval)
// {
//  	 TODO Phase 2 
// 	struct TCB* temp;
// 	queue_dequeue(readyQueue, (void**) &temp);
// 	queue_enqueue(readyQueue, &runningThread);
// 	uthread_ctx_switch(&runningThread.context, &temp->context);
// 	runningThread = (*temp); 
	
//  	/* TODO Phase 3 */
//  	return 0;
// }

