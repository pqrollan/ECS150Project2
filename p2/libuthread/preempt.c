#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sa;
struct itimerval timer;

static void handler(){
	printf("handler ----------------------------------------- fired\n");
	uthread_yield();
	
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	//sigprocmask (SIG_BLOCK, &sa, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	//sigprocmask (SIG_UNBLOCK, &sa, NULL);
}

void preempt_start(void)
{
	/* TODO Phase 4 */

	sigemptyset (&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = &handler;

	if (sigaction(SIGVTALRM, &sa, NULL)==-1){
		printf("error with sig action");
	}

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);

	// while(1){
	// 	pause();
	// }

}


