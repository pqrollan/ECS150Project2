
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
	uthread_t dependent;

};

struct TCB* runningThread;

struct TCB* tcbArray[USHRT_MAX]= {NULL};

/* TODO Phase 2 */


void uthread_yield(void)
{
	preempt_disable();
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	preempt_enable();
	//struct TCB* curr_context = (struct TCB*) malloc (sizeof(struct TCB));
	uthread_t runningThreadTid = runningThread->tid;

	preempt_disable();
	queue_enqueue(readyQueue, (void *)tcbArray[runningThreadTid]); //Enqueue the running thread to the ready queue
	queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
	runningThread = next_thread;
	preempt_enable();	
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); //Switch the contexts
	 //Switch the running thread
}

static void uthread_join_yield(void)
{
	preempt_disable();
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	preempt_enable();
	//struct TCB* curr_context = (struct TCB*) malloc (sizeof(struct TCB));
	uthread_t runningThreadTid = runningThread->tid;

	preempt_disable();
	tcbArray[runningThreadTid]->state= BLOCKED;
	queue_enqueue(blockedQueue, (void *)tcbArray[runningThreadTid]); //Enqueue the running thread to the ready queue
	queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
	runningThread = next_thread;
	preempt_enable();
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); //Switch the contexts
	 //Switch the running thread
}

uthread_t uthread_self(void)
{
	return runningThread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	preempt_enable();
	if(tid_count < USHRT_MAX){
		preempt_disable();
		uthread_ctx_t* c = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
		void* s = uthread_ctx_alloc_stack();
		struct TCB* t = (struct TCB*) malloc (sizeof(struct TCB));
		preempt_enable();
		
		if(tid_count == 0){
			preempt_disable();
			uthread_ctx_t* c_main = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
			void* s_main = uthread_ctx_alloc_stack();
			runningThread = (struct TCB*) malloc (sizeof(struct TCB));
			uthread_ctx_switch(c_main, c_main);
			*runningThread = (struct TCB){0, s_main, c_main, RUNNING, -1, 0};

			readyQueue = queue_create();
			blockedQueue = queue_create();
			tcbArray[tid_count]= runningThread;
			tid_count++;
			preempt_start();
			preempt_enable();
		}
		uthread_ctx_init(c, s, func, arg);
		struct TCB temp= {tid_count, s, c, READY, -1, 0};
		*t = temp;
		preempt_disable();
		queue_enqueue(readyQueue, t);
		tcbArray[tid_count]= t;
		tid_count++;
		preempt_enable();
		return t->tid;
	
	}
	return -1;
}


void uthread_exit(int retval)
{
	struct TCB* next_thread;
	//Set the running thread's return value to retval
	preempt_disable();
	runningThread->retval = retval;
	//Set the status of the thread to terminated
	runningThread->state = ZOMBIE;
	//Switch to the next ready thread
	preempt_enable();
	uthread_t runningThreadTid = runningThread->tid;

	if(runningThread->dependent!=0){
		preempt_disable();
		tcbArray[runningThread->dependent]->state = READY;
		queue_enqueue(readyQueue, tcbArray[runningThread->dependent]);
		queue_delete(blockedQueue, tcbArray[runningThread->dependent]);
		preempt_enable();
	}

	if (queue_length(readyQueue)>0){
		preempt_disable();
		queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
		runningThread = next_thread; //Switch the running thread
		preempt_enable();
		uthread_ctx_switch(tcbArray[runningThreadTid]->context, runningThread->context); //Switch the contexts
		
	}
	else{
		if (runningThread->tid==0){
			preempt_disable();
 			queue_destroy(readyQueue);
			preempt_enable();
		}
		else{
			preempt_disable();
			runningThread = tcbArray[0];
			preempt_enable(); 
			uthread_ctx_switch(tcbArray[runningThreadTid]->context, runningThread->context);
		}
		
	}
	
}


int uthread_join(uthread_t tid, int *retval)
{
	
 	//Ensure that the thread can be joined
	if (tid == 0 || tid == runningThread->tid ||
		tcbArray[tid] == NULL || tcbArray[tid]->dependent!=0){
		return -1;
	}

	//If the thread to be joined is still active, the running thread is now blocked.
	if (tcbArray[tid]->state!= ZOMBIE){
		preempt_disable();
		tcbArray[tid]->dependent= runningThread->tid;
		preempt_enable();
		uthread_join_yield();
	}


	//Populate retval with the return value, if it exists
	if(retval){
		*retval = tcbArray[tid]->retval;
	}
 	
 	//Once the thread is joined, free its dynamically allocated memory
	preempt_disable();
	free(tcbArray[tid]->context);
	uthread_ctx_destroy_stack(tcbArray[tid]->stack);
	free(tcbArray[tid]);
	tcbArray[tid]= NULL;
	printf("process finishes\n");
	preempt_enable();


 	return 0;
}

