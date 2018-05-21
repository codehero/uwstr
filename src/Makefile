NAME=uwstr
INC=internal.h uwstr.h

CFLAGS+=-std=gnu11 -g -Wall
CXXFLAGS+=-std=gnu++14 -g -Wall

DEPEND = $(SOURCES:.c=.d)

OBJECTS = $(SOURCES:.c=.o)

CC       = $(CROSS_COMPILE)gcc
CXX      = $(CROSS_COMPILE)g++
AR       = $(CROSS_COMPILE)ar
RANLIB   = $(CROSS_COMPILE)ranlib

LIB=lib$(NAME).a

all : $(LIB)

header_install :
	mkdir -p $(INC_DEST)/$(NAME)
	cp $(INC) $(INC_DEST)/$(NAME)


$(LIB) : uwstr.o
	$(AR) ru $@ $^
	$(RANLIB) $@

%.o : %.c
	$(CC) -Wall $(CFLAGS) -c -o $@ $<

clean:
	rm -rf *.o bin *.a *.so
