CC=gcc
MKDIR=mkdir

# Testing environment
ifeq ($(shell getconf LONG_BIT),64)
	CFLAGS=-DENVIRONMENT_64
else
	CFLAGS=-DENVIRONMENT_32
endif

ifneq ($(CFG),release)
	CFLAGS+=-W -Wall -DDEBUG -D_GNU_SOURCE -g
else
	CFLAGS+=-W -Wall -Werror -D_GNU_SOURCE -O2
endif
INCPATH=
LDFLAGS=

# Common objects
OBJS=src/main.o \
     src/ines.o \
     src/mappers/mappers.o \
     src/mappers/nrom.o \
     src/mappers/mmc1.o \
     src/mappers/mmc3.o \
     src/mappers/mapper070.o \
     src/utils/list.o

all: nesexp

nesexp: $(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/main.o: $(SRCDIR)/main.c
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c $(SRCDIR)/%.h
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/mappers/%.c $(SRCDIR)/mappers/%.h
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/mappers/%.c
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/utils/%.c $(SRCDIR)/utils/%.h
	$(CC) $(INCPATH) -o $@ -c $< $(CFLAGS)

clean:
	-rm -f $(OBJS)

mrproper: clean
	-rm -f nesexp

