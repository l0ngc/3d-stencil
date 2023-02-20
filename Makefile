#!/bin/bash
# CXX=aarch64-linux-gnu-g++
# CXX=g++
CXX=aarch64-linux-gnu-gcc
all: stencil-3d stencil-3d_op stencil-3d_op_manul stencil-3d_op_unroll stencil-3d_op_fuse stencil-3d_openmp

IDIR=include
CXXFLAGS=-I$(IDIR)

ODIR=src
LDIR =../lib

LIBS=-lm -static -fopemp

stencil-3d.o:
	$(CXX) -c -o $@ stencil-3d.c ${CXXFLAGS}

stencil-3d: stencil-3d.o
	$(CXX) -o $@ $^ $(LIBS)

stencil-3d_op.o:
	$(CXX) -march=armv8-a+sve -c -o $@ stencil-3d_op.c ${CXXFLAGS} -O3

stencil-3d_op: stencil-3d_op.o
	$(CXX) -march=armv8-a+sve -o $@ $^ $(LIBS)

stencil-3d_op_fuse.o:
	$(CXX) -march=armv8-a+sve -c -o $@ stencil-3d_op.c ${CXXFLAGS} -O3 -DFUSE

stencil-3d_op_fuse: stencil-3d_op_fuse.o
	$(CXX) -march=armv8-a+sve -o $@ $^ $(LIBS)

stencil-3d_op_manul.o:
	$(CXX) -fno-tree-vectorize  -march=armv8-a+sve -c -o $@ stencil-3d_op.c ${CXXFLAGS} -O3 -DMANUL

stencil-3d_op_manul: stencil-3d_op_manul.o
	$(CXX) -fno-tree-vectorize  -march=armv8-a+sve -o $@ $^ $(LIBS)

stencil-3d_op_unroll.o:
	$(CXX) -march=armv8-a+sve  -c -o $@ stencil-3d_op.c ${CXXFLAGS} -O3 -DUNROLL

stencil-3d_op_unroll: stencil-3d_op_unroll.o
	$(CXX) -march=armv8-a+sve -o $@ $^ $(LIBS)

stencil-3d_openmp.o:
	$(CXX) -march=armv8-a+sve  -c -o $@ stencil-3d_openmp.c ${CXXFLAGS} -O3 -fopemp

stencil-3d_openmp: stencil-3d_op_unroll.o
	$(CXX) -march=armv8-a+sve -o $@ $^ $(LIBS) 





default: clean all

clean:
	rm -f stencil-3d.o stencil-3d stencil-3d_op.o stencil-3d_op stencil-3d_op_manul stencil-3d_op_manul.o stencil-3d_op_unroll stencil-3d_op_unroll.o stencil-3d_op_fuse.o stencil-3d_op_fuse

#-fno-tree-vectorize 