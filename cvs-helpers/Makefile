#
#  $Id$
#

INSTALL_DIR=../bin
SCRIPTS=cvscommit mkChangeLogList prepend

all: prep ${SCRIPTS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

clean:

install:
	for i in ${SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

