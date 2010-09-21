#!/bin/sh
name = eread
fontes = \
         src/Emain.c\
	 src/ERead.c\
	 src/Ewrite.c
.SUFFIXES: .c .h .o
OS= $(shell uname -s)
CFLAGS=-I./include/ -O2 
LDFLAGS=-L./lib

OBJS= $(fontes:%.c=%.o)

build:	$(OBJS) 
	ls bin || mkdir -p bin
	$(CC)  $(OBJS)  -o bin/$(name)  $(LDFLAGS) -lexoIIv2c -lnetcdf 
list:
	(cd dist/$(projeto); ls $(projeto)_*.md5  \
	| sort  -r > files_$(projeto))
tags:	
	rm -f tags
	ctags -R src/*.c include/*.h

.PHONY: clean
clean:  
	rm src/*.o
	ls bin/ && rm bin/$(name)

# DO NOT DELETE
