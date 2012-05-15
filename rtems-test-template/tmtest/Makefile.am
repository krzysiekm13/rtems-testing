MANAGERS = io rate_monotonic semaphore

rtems_tests_PROGRAMS = @LOWER@
@LOWER@_SOURCES  = init.c
@LOWER@_SOURCES += ../include/timesys.h
@LOWER@_SOURCES += ../../support/src/tmtests_empty_function.c
@LOWER@_SOURCES += ../../support/src/tmtests_support.c

dist_rtems_tests_DATA = @LOWER@.doc

include $(RTEMS_ROOT)/make/custom/@RTEMS_BSP@.cfg
include $(top_srcdir)/../automake/compile.am
include $(top_srcdir)/../automake/leaf.am

OPERATION_COUNT = @OPERATION_COUNT@
AM_CPPFLAGS += -I$(top_srcdir)/include -DOPERATION_COUNT=$(OPERATION_COUNT)
AM_CPPFLAGS += -I$(top_srcdir)/../support/include

LINK_OBJS = $(@LOWER@_OBJECTS) $(@LOWER@_LDADD)
LINK_LIBS = $(@LOWER@_LDLIBS)

@LOWER@$(EXEEXT): $(@LOWER@_OBJECTS) $(@LOWER@_DEPENDENCIES)
	@rm -f @LOWER@$(EXEEXT)
	$(make-exe)

include $(top_srcdir)/../automake/local.am
