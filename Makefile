#!/bin/bash
# CXX=aarch64-linux-gnu-g++
# CXX=g++
CXX=aarch64-linux-gnu-gcc
all: stencil-3d stencil-3d_op

IDIR=include
CXXFLAGS=-I$(IDIR) -O3

ODIR=src
LDIR =../lib

LIBS=-lm -static

stencil-3d.o:
	$(CXX) -c -o $@ stencil-3d.c ${CXXFLAGS} 

stencil-3d: stencil-3d.o
	$(CXX) -o $@ $^ $(LIBS)

stencil-3d_op.o:
	$(CXX) -march=armv8-a+sve -c -o $@ stencil-3d.c ${CXXFLAGS} 

stencil-3d_op: stencil-3d_op.o
	$(CXX) -march=armv8-a+sve -o $@ $^ $(LIBS)


default: all

clean:
	rm -f stencil-3d.o stencil-3d stencil-3d_op.o stencil-3d_op