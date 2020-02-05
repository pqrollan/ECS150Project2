# Target library

all: libuthread.a

#libuthread: libuthread uthread.c uthread.h context.c context.h preempt.c preempt.h test_queue.c queue.c queue.h
#	gcc -Wall -Werror -Wextra -o libuthread.a uthread.c uthread.h context.c context.h preempt.c preempt.h test_queue.c queue.c queue.h
libuthread.a: uthread.o context.o preempt.o queue.o
	ar rcs $@ $^
	ranlib $@

uthread.o: uthread.c uthread.h context.h preempt.h queue.h
	gcc -Wall -Werror -Wextra -c -o uthread.o uthread.c

context.o: context.c context.h preempt.h uthread.h
	gcc -Wall -Werror -Wextra -c -o context.o context.c 

preempt.o: preempt.c preempt.h uthread.h
	gcc -Wall -Werror -Wextra -c -o preempt.o preempt.c

queue.o: queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o queue.o queue.c

clean:    
	rm -f libuthread.a uthread.o context.o preempt.o test_queue.o queue.o


## TODO: Phase 1.1
