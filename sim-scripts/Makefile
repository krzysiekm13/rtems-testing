#
#  $Id$
#

INSTALL_DIR=../bin
GDBSIM_SCRIPTS=h8sim h8sim-gdb jmr3904 jmr3904-gdb \
  m32csim m32csim-gdb m32rsim m32rsim-gdb psim psim-gdb \
  sis sis-gdb simsh simsh-gdb
SKYEYE_SCRIPTS=ant5206 bf537Stamp csb337 csb350 csb360 edb7312 ezkit533 \
  rtl22xx smdk2410 leon2-skyeye
TSIM_SCRIPTS=erc32 leon2 leon3
QEMU_SCRIPTS=qemu-rtems
GENERATED_SCRIPTS=${GDBSIM_SCRIPTS} ${SKYEYE_SCRIPTS} \
  ${TSIM_SCRIPTS} ${QEMU_SCRIPTS}

COMPILED_PROGRAMS=usleep
UNPROCESSED_SCRIPTS=nosim

all: prep ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} \
     ${UNPROCESSED_SCRIPTS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

usleep: usleep.c
	$(CC) -o $@ $<

### QEMU BSPs
qemu-rtems: gdb-sim-run.in qemu-rtems.in
	./mkrun no i386 i386 qemu-rtems

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

leon2-skyeye: gdb-sim-run.in leon2-skyeye.in
	./mkrun no SPARC sparc leon2-skyeye

### TSIM Simulator BSPs
erc32: gdb-sim-run.in erc32.in
	./mkrun no SPARC sparc erc32

leon2: gdb-sim-run.in leon2.in
	./mkrun no SPARC sparc leon2

leon3: gdb-sim-run.in leon3.in
	./mkrun no SPARC sparc leon3

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
	rm -f ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS}

install:
	for i in ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} \
                 ${UNPROCESSED_SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

