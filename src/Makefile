CC=clang -Wall
# to be replaced by your own file
TARGET=project

all: ${TARGET}

${TARGET}: ${TARGET}.o inout.o

inout.o: inout.c

clean:
	rm -f *.o *~ ${TARGET}
