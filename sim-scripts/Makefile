#
#  $Id$
#

INSTALL_DIR=../bin
GENERATED_SCRIPTS=jmr3904-4.10 sis-4.10
COMPILED_PROGRAMS=usleep

all: prep ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

usleep: usleep.c
	$(CC) -o $@ $<

jmr3904-4.10: gdb-sim-run.in jmr3904.in
	sed -e 's/@CPU@/MIPS/' \
	    -e 's/@CPU_TARGET@/mipstx39/' \
	    -e 's/@BSP@/jmr3904/' \
	    -e '/^##INSERT BSP SUPPORT HERE/r jmr3904.in' <$< >$@
	chmod +x $@

sis-4.10: gdb-sim-run.in sis.in
	sed -e 's/@CPU@/SPARC/' \
	    -e 's/@CPU_TARGET@/sparc/' \
	    -e 's/@BSP@/sis/' \
            -e '/^##INSERT BSP SUPPORT HERE/r sis.in' <$< >$@
	chmod +x $@

clean:
	rm -f ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS}

install:
	for i in ${GENERATED_SCRIPTS} ${COMPILED_PROGRAMS} ; do \
	  cp $${i} ${INSTALL_DIR} ; \
	done
	cd ${INSTALL_DIR} ; for i in jmr3904 ; do \
	  for v in 4.9 4.8 4.7 4.6 ; do \
	    rm -f $${i}-$${v} ; ln -s $${i}-4.10 $${i}-$${v} ; \
	done ;\
	done
	
