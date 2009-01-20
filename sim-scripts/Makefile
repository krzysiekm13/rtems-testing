#
#  $Id$
#

INSTALL_DIR=../bin
GENERATED_SCRIPTS=h8sim h8sim-gdb jmr3904 jmr3904-gdb m32csim m32csim-gdb \
  m32rsim m32rsim-gdb psim psim-gdb sis sis-gdb simsh simsh-gdb
COMPILED_PROGRAMS=usleep

all: prep ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

usleep: usleep.c
	$(CC) -o $@ $<

h8sim h8sim-gdb: gdb-sim-run.in gdb-sim.in h8sim.in
	./mkrun H8300 h8300 h8sim

jmr3904 jmr3904-gdb: gdb-sim-run.in gdb-sim.in jmr3904.in
	./mkrun MIPS mipstx39 jmr3904

m32csim m32csim-gdb: gdb-sim-run.in gdb-sim.in m32csim.in
	./mkrun M32C m32c m32csim

m32rsim m32rsim-gdb: gdb-sim-run.in gdb-sim.in m32rsim.in
	./mkrun M32R m32r m32rsim

psim psim-gdb: gdb-sim-run.in gdb-sim.in psim.in
	./mkrun PowerPC powerpc psim

sis sis-gdb: gdb-sim-run.in gdb-sim.in sis.in
	./mkrun SPARC sparc sis

simsh simsh-gdb: gdb-sim-run.in gdb-sim.in simsh.in
	./mkrun SuperH sh simsh

clean:
	rm -f ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS}

install:
	for i in ${COMPILED_PROGRAMS} ; do \
	  cp $${i} ${INSTALL_DIR} ; \
	done
	for i in ${GENERATED_SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i}-4.10 ; \
	  chmod +x ${INSTALL_DIR}/$${i}-4.10 ; \
	done
	cd ${INSTALL_DIR} ; for i in ${GENERATED_SCRIPTS} ; do \
	  for v in 4.9 4.8 4.7 4.6 ; do \
	    rm -f $${i}-$${v} ; ln -s $${i}-4.10 $${i}-$${v} ; \
	done ;\
	done
	
