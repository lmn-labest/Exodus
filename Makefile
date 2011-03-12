#!/bin/sh
name = eread
CC=gcc
CXX=g++
fontes = \
         src/Emain.c\
	 src/ERead.c\
	 src/Ewrite.c\
	 src/Propnode.c
.SUFFIXES: .c .h .o
OS= $(shell uname -s)
#CFLAGS=-I./include/ -O2 -D_DEBUG 
CFLAGS=-I./include/ -O2  
CXXFLAGS=-I./include/ -O2  
LDFLAGS=-L./lib -lm -lexoIIv2c -lgidpost -lnetcdf

OBJS= $(fontes:%.c=%.o)

build:	$(OBJS) 
	ls bin || mkdir -p bin
	$(CXX) $(CXXFLAGS)  $(OBJS) -Wall -o bin/$(name) $(LDFLAGS)  
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
