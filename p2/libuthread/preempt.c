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

bool signal_enabled = true;

static void handler(){
	// if(signal_enabled){
	// 	uthread_yield();
	// }
	printf("handler fired\n");
}

void preempt_disable(void)
{
	/* TODO Phase 4 */
	signal_enabled = false;
}

void preempt_enable(void)
{
	/* TODO Phase 4 */
	signal_enabled = true;

}

void preempt_start(void)
{
	/* TODO Phase 4 */
	signal(SIGVTALRM, handler);

	// //Set up timer
	// while(true){
		

	// }
	struct sigaction sa;
	sa.sa_handler = handler;
	// sa.sa_mask = 0;
	// sa.sa_flags = 0;
	sigaction(SIGALRM, &sa, NULL);

	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 10000;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 100000;
	setitimer(ITIMER_REAL, &timer, NULL);

	// while(1){
	// 	pause();
	// }

}


