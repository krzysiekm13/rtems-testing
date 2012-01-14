#
#  $Id: Makefile,v 1.3 2010/06/21 12:37:03 joel Exp $
#

INSTALL_DIR=../bin
SCRIPTS=commit-with-changelog-diff commit mkChangeLogList prepend mktest

all: prep ${SCRIPTS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

clean:

install:
	for i in ${SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

