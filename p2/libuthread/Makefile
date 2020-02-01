# Target library
lib := libuthread.a

all: libuthread $(lib)

libuthread.a: test_queue.o queue.o
	gcc -Wall -Werror -Wextra -o libuthread test_queue.o queue.o

test_queue.o: test_queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o test_queue.o test_queue.c

queue.o: queue.c queue.h
	gcc -Wall -Werror -Wextra -c -o queue.o queue.c


## TODO: Phase 1.1
