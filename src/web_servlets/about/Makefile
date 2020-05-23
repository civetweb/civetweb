.PHONY: target

cc=gcc
PWD=`pwd`
includedir=${PWD}/../../../include

target_obj=builtin.o

src=$(wildcard *.c)
obj=$(patsubst %.c, %.o, $(src))

CFLAGS+=-I${includedir} -fPIC
LDFLAGS+=-r

target: ${target_obj}

${target_obj}: ${obj}
	${cc} -o $@ ${LDFLAGS} $^

.SUFFIXES: .c
.c.o:
	${cc} -c $< ${CFLAGS}

clean:
	rm -rf ${obj} ${target_obj}