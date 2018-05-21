CFLAGS+=-std=gnu11 -g -Wall -I$(INCDIR)
LDFLAGS+=-L$(LIBDIR)

DEPEND = $(SOURCES:.c=.d)

OBJECTS = $(SOURCES:.c=.o)

CC       = $(CROSS_COMPILE)gcc
CXX      = $(CROSS_COMPILE)g++
AR       = $(CROSS_COMPILE)ar
RANLIB   = $(CROSS_COMPILE)ranlib

all : uwstr_test

uwstr_test : uwstr_test.o
	$(CC) $(LDFLAGS) -o $@ $< -luwstr

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o bin *.a *.so
