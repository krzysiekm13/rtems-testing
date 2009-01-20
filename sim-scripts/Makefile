#
#  $Id$
#

INSTALL_DIR=../bin
GENERATED_SCRIPTS=h8sim h8sim-gdb jmr3904 jmr3904-gdb \
  m32csim m32csim-gdb m32rsim m32rsim-gdb psim psim-gdb \
  sis sis-gdb simsh simsh-gdb
SKYEYE_SCRIPTS=edb7312
COMPILED_PROGRAMS=usleep

all: prep ${GENERATED_SCRIPTS} ${SKYEYE_SCRIPTS} ${COMPILED_PROGRAMS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

usleep: usleep.c
	$(CC) -o $@ $<

### Skyeye Simulator BSPs
edb7312: gdb-sim-run.in edb7312.in
	./mkrun no ARM arm edb7312

### GDB Simulator BSPs
h8sim h8sim-gdb: gdb-sim-run.in gdb-sim.in h8sim.in
	./mkrun yes H8300 h8300 h8sim

jmr3904 jmr3904-gdb: gdb-sim-run.in gdb-sim.in jmr3904.in
	./mkrun yes MIPS mipstx39 jmr3904

m32csim m32csim-gdb: gdb-sim-run.in gdb-sim.in m32csim.in
	./mkrun yes M32C m32c m32csim

m32rsim m32rsim-gdb: gdb-sim-run.in gdb-sim.in m32rsim.in
	./mkrun yes M32R m32r m32rsim

psim psim-gdb: gdb-sim-run.in gdb-sim.in psim.in
	./mkrun yes PowerPC powerpc psim

sis sis-gdb: gdb-sim-run.in gdb-sim.in sis.in
	./mkrun yes SPARC sparc sis

simsh simsh-gdb: gdb-sim-run.in gdb-sim.in simsh.in
	./mkrun yes SuperH sh simsh

clean:
	rm -f ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} ${SKYEYE_SCRIPTS}

install:
	for i in ${COMPILED_PROGRAMS} ${SKYEYE_SCRIPTS}; do \
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
	
