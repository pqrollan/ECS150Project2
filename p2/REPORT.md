Project 2
High-Level Description
This project is a threading library. It supports creation of threads that
 and yield, and preempting alleviates the problem of uncooperative threads.
 Stack management, context management and switching between functions are
 implemented using the ucontext library.

Implementation Decisions and Details

Queue:
The queue is implemented using a doubly linked list. A queue struct contains a
pointer to its first and last elements as well as the length of the queue. A
node of the queue contains pointers to its previous and next elements as well
as a void pointer to hold a pointer to any data type desired. The doubly linked
list pattern was implemented to simplify linking and unlinking. The queue’s
pointer to first and last elements helped us ensure that dequeue and enqueue
are O(1) operations.
Queue nodes are allocated during queue_enqueue and freed during queue_dequeue
or queue_delete. The queue struct is freed during queue_destroy().
Each function performs input validation as per the specifications in queue.h

To test the queue, we devised unit tests that ensured the correct behavior
under invalid input, as well as testing the behavior of the queue under various
conditions using asserts.
We tested various creation and destroy patterns, making sure the destroy
function cannot execute on a null pointer.
We tested enqueuing and dequeuing to get various queue lengths.
We tested iteration using the professors function and a modified version of our
own.
Finally we unit tested all the possible faulty inputs for each function.


Uthread
Threads are represented using the private TCB data structure, which contains:
A thread’s ID
Pointers to its stack and context
Its state. A thread can be either Running, Ready, Blocked, or a Zombie.
Its return value, which will be populated when the thread’s function returns
The ID of a dependent thread. If thread T1 has called join on T2, T1 will be
set as T2’s dependent.
The current running thread is stored as a global variable.
An array of all existing threads indexed by their IDs is used to ensure easy
access to a particular thread. This ensures easy O(1) access to any particular
thread. This is useful when threads need to be accessed given their IDs in the
function uthread_join(), when threads need to be removed from the blocked queue
in the uthread_exit() function, and in other areas.

Two TCB queues are used: a queue of threads and their additional information in
the Ready state, and a queue of those that are Blocked.
These queues are used when switching between threads. For example, during
yield, control must switch to the oldest thread in the Ready queue, which is
simply dequeued from the Ready queue (as it is a FIFO queue design) and using
the dequeue return value we can get the desired TCB. From there we assign our
global TCB* runningThread to point to this thread, and context switch to it.
When the main thread is created, these queues are allocated and initialized.
All resources associated with a TCB are freed when a thread joins it and the
thread has gotten a chance to run and give the joining thread its return value.


Uthread was tested using the professor’s two examples, as well as with many of
our own test cases. Print statements track the threads’ execution, ensuring
that the control flow is correct.

Preempt

We implemented thread preemption using signals. We used the sigaction struct to
manage signals and the setitimer function to create a virtual timer that fires
a SIGVTALRM signal every 10000 microseconds (equivalent to 100 Hertz). The
sigaction’s mask is set to be capable of blocking SIGVTALRM signals. The signal
handler calls uthread_yield, so every 10000 seconds, the current running thread
will yield, preventing uncooperative threads from hogging resources.
Preemption can be disabled by blocking the SIGVTALRM signals with the
preempt_disable() function, preventing the yield from occurring. Preemption was
enabled or disabled in uthread.c before any global variables were accessed or
data when data was allocated or freed. This ensures that changes to shared data
structures were never compromised by unexpected context switches.

We tested preemption by proving that an uncooperative thread will be forcefully
yielded to a ready thread. To do this, from the main we created two threads.
The first thread loops endlessly until a certain global variable is set to 1.
The second thread, sets this variable to 1. Neither of these threads contains a
call to yield. Because thread 1 is created before thread 2, it will be locked
in an infinite loop until thread 2 changes the value of the global variable.
Preempt will eventually be called, forcefully yielding thread 1 into thread 2
(the next available thread). Thread 2 finally sets the global variable to 1,
exits, and allows the program to go back to thread 1. Thread 1 now has the
value it needs! We discovered that thread 1l used an old, probably cashed,
version of the global variable, and remained stuck in the loop despite thread 2
executing and printing proof. To work around this, we printed the variable in
the while loop causing thread 1 to see when the variable was updated. Our test
asserts that the loop completes, proving that thread 1 must have yielded to
thread 2, and that preemption is succeeding.
