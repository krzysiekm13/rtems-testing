#
#  $Id$
#

INSTALL_DIR=../bin
SCRIPTS=commit-with-changelog-diff cvscommit mkChangeLogList prepend mktest

all: prep ${SCRIPTS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

clean:

install:
	for i in ${SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

