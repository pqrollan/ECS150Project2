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


enum uthread_State {RUNNING, READY, BLOCKED, ZOMBIE};

int tid_count=0;
queue_t readyQueue;
queue_t blockedQueue;



struct TCB {
	uthread_t tid;
	void* stack;
	uthread_ctx_t context;
	enum uthread_State state;

};

struct TCB mainThread;

/* TODO Phase 2 */

// void uthread_yield(void)
// {
// 	/* TODO Phase 2 */
// }

// uthread_t uthread_self(void)
// {
// 	/* TODO Phase 2 */
// }

int uthread_create(uthread_func_t func, void *arg)
{
	if(tid_count < USHRT_MAX){
		//If main:
		uthread_ctx_t c;
		void* s = uthread_ctx_alloc_stack();
		if(tid_count == 0){
			uthread_ctx_switch(&c, &c);
			struct TCB t = {0, s, c, RUNNING };
			mainThread = t;
			readyQueue = queue_create();
			blockedQueue = queue_create();
			//Enqueue to Running queue
		}
		else{
			uthread_ctx_init(&c, s, func, arg);
			struct TCB thread = {tid_count++, s, c, READY };
			queue_enqueue(readyQueue, &thread);
			//Enqueue to Ready queue
		}
	}
	
}

// void uthread_exit(int retval)
// {
// 	/* TODO Phase 2 */
// }

// int uthread_join(uthread_t tid, int *retval)
// {
// 	/* TODO Phase 2 */
// 	/* TODO Phase 3 */
// }

