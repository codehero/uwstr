NAME=uwstr
LIB=lib$(NAME).a

all : lib/$(LIB)

test : test/uwstr_test

test/uwstr_test : lib/$(LIB) 
	echo blah
	INCDIR=../include LIBDIR=../lib make -C test all

src/$(LIB):
	make -C src

lib/$(LIB) : src/$(LIB)
	mkdir -p include lib
	cp src/$(LIB) lib/$(LIB)
	INC_DEST=../include make -C src header_install

clean:
	make -C src clean
	make -C test clean
	rm -rf include lib bin
