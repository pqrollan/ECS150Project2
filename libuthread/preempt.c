#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <string.h>


#include "preempt.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

//Respond to SIGVTALRM signals by yielding
void handler(){
	uthread_yield();
	
}

void preempt_disable(void)
{
	//Unblock the SIGVTALRM signal
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset (&mask, SIGVTALRM);
	sigprocmask (SIG_BLOCK, &mask, NULL);
}

void preempt_enable(void)
{
	//Block the SIGVTALRM signal
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset (&mask, SIGVTALRM);
	sigprocmask (SIG_UNBLOCK, &mask, NULL);
}

void preempt_start(void)
{
	// Set a sigaction struct, add SIGVTALRM to its mask, and set its handler
	// to handler().
	struct sigaction sig_a;
	struct itimerval timer;
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset (&mask, SIGVTALRM);

	memset(&sig_a, 0, sizeof(sig_a));
	sig_a.sa_handler = &handler;
	sigaction(SIGVTALRM, &sig_a, NULL);

	//Set a virtual interval timer that fires a SIGVTALRM signal every 10000 microseconds. 
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 10000;
	setitimer(ITIMER_VIRTUAL, &timer, NULL);
	
}


