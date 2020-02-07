#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sig_a;
struct itimerval timer;

static void handler(){
	printf("handler ----------------------------------------- fired\n");
	uthread_yield();
	
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	sigaddset (&sig_a.sa_mask, SIGVTALRM);
	sigprocmask (SIG_BLOCK, &sig_a.sa_mask, NULL);
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	
	sigaddset (&sig_a.sa_mask, SIGVTALRM);
	sigprocmask (SIG_UNBLOCK, &sig_a.sa_mask, NULL);
}

void preempt_start(void)
{
	printf("Inside preempt\n");
	/* TODO Phase 4 */

	sig_a.sa_flags = 0;
	sig_a.sa_handler = handler;
	sigemptyset(&sig_a.sa_mask);

	sigaction(SIGVTALRM, &sig_a, NULL);
	preempt_enable();

	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000; //10000
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
	/*
	sigset_t mask;
	sigprocmask(0, NULL, &mask);
	sigdelset(&mask, SIGVTALRM);
	sigsuspend(&mask);
	*/
	//while(1);
	
}


