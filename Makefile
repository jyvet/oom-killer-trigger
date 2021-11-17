multirw:
	gcc -Wall -O3 -o oom-killer-trigger oom-killer-trigger.c

all: oom-killer-trigger

clean:
	-@rm -f oom-killer-trigger
