CXX=aarch64-linux-gnu-gcc
all: stencil stencil_op

stencil: stencil-3d.c
	${CXX} -O3 -fopenmp -march=armv8-a+sve stencil-3d.c -o stencil

stencil_op: stencil-3d.c
	${CXX} -O3 -fopenmp -march=armv8-a+sve stencil-3d_op.c -o stencil_op

run_all: run run_op

default: all

clean:
	rm -f stencil_op stencil
