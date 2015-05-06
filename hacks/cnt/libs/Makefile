AR=ar
CC=gcc
MKDIR=mkdir

CFLAGS=-W -Wall -Werror -D_GNU_SOURCE -O2

# Testing environment
ifeq ($(shell getconf LONG_BIT),64)
	CFLAGS+=-DENVIRONMENT_64
else
	CFLAGS+=-DENVIRONMENT_32
endif

OBJS=utf8.o

all: libutf8.a

libutf8.a: $(OBJS)
	ar rcu $@ $(OBJS)
	ranlib $@

utf8.o: utf8.c utf8.h
	$(CC) -c $(CFLAGS) $(INCPATH) -o $@ $<

clean:
	-rm -f $(OBJS)

mrproper: clean
	-rm -f libutf8.a

