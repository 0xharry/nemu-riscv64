.PHONY: test testrun clean

vpath %.c /home/harry/ics2020/abstract-machine/klib/src
vpath %.h /home/harry/ics2020/abstract-machine/klib/include
CFLAGS = -D_KLIB_TEST_

test : main.c stdio.c string.c klib_test.h
	gcc -g  $(CFLAGS) $+ -o $@

testrun : test
	./test

clean :
	rm test