#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>


/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100

struct sigaction sa;
struct itimerval timer;

static void handler(){
    printf("handler ----------------------------------------- fired\n");
    
}

void preempt_disable(void)
{
    /* TODO Phase 4 */
    sigprocmask (SIG_BLOCK, &sa.sa_mask, NULL);
}

void preempt_enable(void)
{
    /* TODO Phase 4 */
    sigprocmask (SIG_UNBLOCK, &sa.sa_mask, NULL);
}

void preempt_start(void)
{
    /* TODO Phase 4 */
    //sigset_t set;
    //sigaddset(&set, SIGVTALRM);
    sigemptyset (&sa.sa_mask);
    // sa.sa_mask = set;
    sa.sa_flags = 0;
    sa.sa_handler = &handler;
    preempt_enable();

    if (sigaction(SIGVTALRM, &sa, NULL)==-1){
        printf("error with sig action");
    }

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 10000; //10000
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 10000;
    if( setitimer(ITIMER_VIRTUAL, &timer, NULL) < 0){
        printf("Don't have a timer\n");
    }

    // while(1){
    //  pause();
    // }
}
void func(){

    preempt_start();

}


int main(int argc, char *argv[]){
    func();

    while (1) {
     
    }
    // return 0;
}
