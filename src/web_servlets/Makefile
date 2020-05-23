.PHONY: target

ASHIBALIB=libashiba.so

PWD=`pwd`
includedir=${PWD}/../../include

kitasunalib=libkitasuna.so
kitasuna=-lkitasuna
kitasunadir=${PWD}/kitasuna
kitasunaincdir=${kitasunadir}/include
kitasunalibdir=${kitasunadir}/lib

cc=gcc
CFLAGS= -I${includedir} -I${kitasunaincdir} -fPIC
LDFLAGS=-L${kitasunalibdir}  -fPIC -shared ${kitasuna}
installdir?=${PWD}/out

subdir=auth about article catagory
subdir_o=$(addsuffix /builtin.o, ${subdir})

objfiles=servlets.o

target: ${ASHIBALIB}

${subdir_o}: ${subdir}

.SUFFIXES: .c
.c.o:
	${cc} -c $< ${CFLAGS}

${subdir}: ECHO
	make -C $@
ECHO:
	@echo $(SUBDIRS)

${ASHIBALIB}: ${objfiles} ${subdir_o}
	${cc} -o $@ ${LDFLAGS} $^

install: ${ASHIBALIB}
	install -d -m 0755 "${installdir}"
	install -m 755 ${ASHIBALIB} ${installdir}
	install -m 755 ${kitasunalibdir}/*.so ${installdir}

clean:
	rm -rf ${objfiles}
	@for dir in ${subdir}; do \
		make -C $${dir} clean; \
	done
