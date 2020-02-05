# Target library
lib := libuthread.a

all: libuthread $(lib)

libuthread.a: uthread.o context.o preempt.o test_queue.o queue.o
	gcc -Wall -Werror -Wextra -o libuthread.a uthread.o context.o test_queue.o queue.o

uthread.o: uthread.c uthread.h context.h preempt.h queue.h
	gcc -Wall -Werror -Wextra -o uthread.o uthread.c

context.o: context.c context.h preempt.h uthread.h
	gcc -Wall -Werror -Wextra -o context.o context.c 

preempt.o: preempt.c preempt.h uthread.h
	gcc -Wall -Werror -Wextra -o preempt.o preempt.c

test_queue.o: test_queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o test_queue.o test_queue.c

queue.o: queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o queue.o queue.c


## TODO: Phase 1.1
