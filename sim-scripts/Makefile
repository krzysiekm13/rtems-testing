#
#  $Id$
#

INSTALL_DIR=../bin
GENERATED_SCRIPTS=h8sim h8sim-gdb jmr3904 jmr3904-gdb \
  m32csim m32csim-gdb m32rsim m32rsim-gdb psim psim-gdb \
  sis sis-gdb simsh simsh-gdb
SKYEYE_SCRIPTS=ant5206 bf537Stamp csb337 csb350 csb360 edb7312 ezkit533 \
  rtl22xx smdk2410
COMPILED_PROGRAMS=usleep

all: prep ${GENERATED_SCRIPTS} ${SKYEYE_SCRIPTS} ${COMPILED_PROGRAMS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

usleep: usleep.c
	$(CC) -o $@ $<

### Skyeye Simulator BSPs
ant5206: gdb-sim-run.in ant5206.in
	./mkrun no M68K-Coldfire m68k ant5206

csb337: gdb-sim-run.in csb337.in
	./mkrun no ARM arm csb337

csb350: gdb-sim-run.in csb350.in
	./mkrun no MIPS mips csb350

csb360: gdb-sim-run.in csb360.in
	./mkrun no M68K-Coldfire m68k csb360

bf537Stamp: gdb-sim-run.in bf537Stamp.in
	./mkrun no Blackfin bfin bf537Stamp

edb7312: gdb-sim-run.in edb7312.in
	./mkrun no ARM arm edb7312

rtl22xx: gdb-sim-run.in rtl22xx.in
	./mkrun no ARM arm rtl22xx

smdk2410: gdb-sim-run.in smdk2410.in
	./mkrun no ARM arm smdk2410

ezkit533: gdb-sim-run.in ezkit533.in
	./mkrun no Blackfin bfin ezkit533

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
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

