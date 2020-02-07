# Target library


objs := uthread.o context.o preempt.o queue.o

CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -c -o

all: libuthread.a

deps := $(patsubst %.o,%.d,$(objs))
-include $(deps)
DEPFLAGS = -MMD -MF $(@:.o=.d)

libuthread.a: $(objs)
	ar rcs $@ $^
	ranlib $@

%.o: %.c
	@echo "CC $@ $^"
	$(CC) $(CFLAGS) $@ $< $(DEPFLAGS)

clean:    
	rm -f libuthread.a $(objs) $(deps)


## TODO: Phase 1.1
