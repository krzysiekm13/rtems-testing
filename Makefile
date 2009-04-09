#
# $Id$
#
#
# NOTE: This is not the fanciest Makefile I have ever written. ;)

all:
	make -C sim-scripts
	make -C cvs-helpers

install:
	make -C sim-scripts install
	make -C cvs-helpers install

clean:
	make -C sim-scripts clean
	make -C cvs-helpers clean
	rm -rf bin

up:
	cvs up -Pd 2>&1 | grep -v ^cvs
