#
# $Id$
#
#
# NOTE: This is not the fanciest Makefile I have ever written. ;)

all:
	make -C covoar
	make -C rtems-coverage
	make -C sim-scripts
	make -C cvs-helpers
	make -C merge-helpers

install:
	make -C covoar install
	make -C rtems-coverage install
	make -C sim-scripts install
	make -C cvs-helpers install
	make -C merge-helpers install

clean:
	make -C covoar clean
	make -C rtems-coverage clean
	make -C sim-scripts clean
	make -C cvs-helpers clean
	make -C merge-helpers clean
	rm -rf bin

up:
	cvs up -Pd 2>&1 | grep -v ^cvs
