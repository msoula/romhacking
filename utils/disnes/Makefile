CC=gcc
MKDIR=mkdir

# Testing environment
ifeq ($(shell getconf LONG_BIT),64)
	CFLAGS=-DENVIRONMENT_64
else
	CFLAGS=-DENVIRONMENT_32
endif

CFLAGS= -W -Wall -DDEBUG -D_GNU_SOURCE -g

SRCDIR=src

OBJS=main.o \
	bank.o \
	command.o \
	ines.o \
	jmps.o \
	opcodes.o \
	disnes_cfg.o \
	config.o \
	ptrlist.o

all: disnes

disnes: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

main.o: $(SRCDIR)/main.c
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

%.o: $(SRCDIR)/utils/%.c $(SRCDIR)/utils/%.h
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

clean:
	rm *.o

mrproper: clean
	rm disnes

