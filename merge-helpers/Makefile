INSTALL_DIR=../bin
SCRIPTS  = check_submission
SCRIPTS += commit-with-changelog-diff
SCRIPTS += commit
SCRIPTS += mkChangeLogList
SCRIPTS += prepend

all: prep ${SCRIPTS} install

prep:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

clean:

install:
	for i in ${SCRIPTS} ; do \
	  cp $${i} ${INSTALL_DIR}/$${i} ; \
	  chmod +x ${INSTALL_DIR}/$${i} ; \
	done

