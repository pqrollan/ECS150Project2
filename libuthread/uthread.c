
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

//Running represents a running thread
//Ready represents a thread that can run
//Blocked represents a thread waiting for another thread to join
// Zombie represents a completed thread whose resources need
// to be freed
enum uthread_State {RUNNING, READY, ZOMBIE, BLOCKED};

//tid count = 0 means that first thread created is the main thread
int tid_count=0;

// Queues that hold TCB struct pointers representing threads in their
// respective states
queue_t readyQueue;
queue_t blockedQueue;

struct TCB {
	uthread_t tid; //Thread ID
	void* stack;
	uthread_ctx_t* context;
	enum uthread_State state;
	int retval; //Value returned by the thread's function, populated in exit
	uthread_t dependent; //ID of thread waiting on this thread to join

};

struct TCB* runningThread; //Currently running thread

 //Array storing all running threads indexed by their Thread ID
struct TCB* tcbArray[USHRT_MAX]= {NULL};


void uthread_yield(void)
{
	preempt_disable();
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	preempt_enable();

	uthread_t runningThreadTid = runningThread->tid;

	preempt_disable();
	//Enqueue the running thread to the ready queue
	queue_enqueue(readyQueue, (void *)tcbArray[runningThreadTid]); 
	//Get the next thread ready to run
	queue_dequeue(readyQueue, (void**) &next_thread); 
	//Reset the currently running thread
	runningThread = next_thread;
	preempt_enable();	

	//Context switch to the new running thread
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); 
}

/*
A special form of yield called when joining threads. Enqueues the thread
waiting on join to the Blocked queue and yields to the next Ready thread.
*/
static void uthread_join_yield(void)
{
	preempt_disable();
	struct TCB* next_thread = (struct TCB*) malloc (sizeof(struct TCB));
	preempt_enable();

	uthread_t runningThreadTid = runningThread->tid;

	preempt_disable();
	//Set the running thread's state to blocked
	tcbArray[runningThreadTid]->state= BLOCKED;
	//Enqueue the running thread to the ready queue
	queue_enqueue(blockedQueue, (void *)tcbArray[runningThreadTid]); 
	//Get the next thread ready to run
	queue_dequeue(readyQueue, (void**) &next_thread); 
	runningThread = next_thread;
	preempt_enable();

	//Context switch to the new running thread
	uthread_ctx_switch((tcbArray[runningThreadTid]->context), (runningThread->context)); 
}

uthread_t uthread_self(void)
{
	return runningThread->tid;
}

int uthread_create(uthread_func_t func, void *arg)
{
	preempt_enable();
	//Ensure that the number of threads hasn't exceeded the maximum
	if(tid_count < USHRT_MAX){

		preempt_disable();
		uthread_ctx_t* c = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
		void* s = uthread_ctx_alloc_stack();
		struct TCB* t = (struct TCB*) malloc (sizeof(struct TCB));
		preempt_enable();
		
		//If the main thread is being created
		if(tid_count == 0){
			preempt_disable();
			//Allocate a new thread and its resources, set it to the running thread
			uthread_ctx_t* c_main = (uthread_ctx_t*)malloc(sizeof(uthread_ctx_t));
			void* s_main = uthread_ctx_alloc_stack();
			runningThread = (struct TCB*) malloc (sizeof(struct TCB));
			uthread_ctx_switch(c_main, c_main);
			*runningThread = (struct TCB){0, s_main, c_main, RUNNING, -1, 0};

			// Initialize the queues. Store this thread in the tcbArray,
			// increment the thread count, and begin preemption.
			readyQueue = queue_create();
			blockedQueue = queue_create();
			tcbArray[tid_count]= runningThread;
			tid_count++;
			preempt_start();
			preempt_enable();
		}
		//Initialize the new running thread.
		uthread_ctx_init(c, s, func, arg);
		struct TCB new_thread = {tid_count, s, c, READY, -1, 0};
		*t = new_thread;
		preempt_disable();
		// Enqueue the new thread to the ready queue. Store the new thread in
		// the tcbArray and increment the total thread count
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
	preempt_disable();
	//Collect the return value and set the thread state to Zombie
	runningThread->retval = retval;
	runningThread->state = ZOMBIE;
	preempt_enable();

	uthread_t runningThreadTid = runningThread->tid;

	// If the running thread is not waiting for main to join, transfer the
	// newly unblocked dependent to the ready queue. 
	if(runningThread->dependent!=0){
		preempt_disable();
		tcbArray[runningThread->dependent]->state = READY;
		queue_enqueue(readyQueue, tcbArray[runningThread->dependent]);
		queue_delete(blockedQueue, tcbArray[runningThread->dependent]);
		preempt_enable();
	}

	//If the ready queue is not empty, switch to the next Ready thread.
	if (queue_length(readyQueue)>0){
		preempt_disable();
		queue_dequeue(readyQueue, (void**) &next_thread); //Get the next thread ready to run
		runningThread = next_thread; //Switch the running thread
		preempt_enable();
		uthread_ctx_switch(tcbArray[runningThreadTid]->context, runningThread->context); //Switch the contexts
		
	}
	else{
		// If the main thread is exiting, deallocate the Ready Queue; the
		// process is complete. Otherwise, main is the only thread remaining,
		// so switch back to it.
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

	// If the thread to be joined is still active, the running thread is now
	// blocked. Set the dependent of the joining thread to the running thread.
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
	preempt_enable();


 	return 0;
}

