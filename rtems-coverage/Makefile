#
# $Id$
#

INSTALL_DIR=../bin
CXXFLAGS=-g -Wall -O2
PROGRAMS=covmerge coverage_converter qemu-dump-trace
SHARED_OBJS=CoverageMap.o CoverageMapBase.o \
  CoverageFactory.o \
  CoverageReaderBase.o \
  CoverageReaderQEMU.o \
  CoverageReaderRTEMS.o \
  CoverageReaderSkyeye.o \
  CoverageReaderTSIM.o \
  CoverageWriterBase.o \
  CoverageWriterRTEMS.o \
  CoverageWriterSkyeye.o \
  CoverageWriterTSIM.o

COVMERGE_OBJS= covmerge.o $(SHARED_OBJS) \
  CoverageRanges.o \
  Explanations.o \
  ObjdumpProcessor.o \
  Reports.o \
  Toolnames.o

COVERAGE_CONVERTER_OBJS=coverage_converter.o $(SHARED_OBJS)

all: $(PROGRAMS) ${INSTALL_DIR} ../bin/coverage_converter ../bin/qemu-dump-trace

../bin/coverage_converter: coverage_converter ${INSTALL_DIR}
	cp coverage_converter ../bin

../bin/qemu-dump-trace: qemu-dump-trace ${INSTALL_DIR}
	cp qemu-dump-trace ../bin

${INSTALL_DIR}:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

qemu-dump-trace: qemu-dump-trace.c ${INSTALL_DIR}
	$(CXX) -o $(@) qemu-dump-trace.c

covmerge: $(COVMERGE_OBJS)
	$(CXX) -o $(@) $(COVMERGE_OBJS)

covmerge.o: covmerge.cc CoverageMap.h CoverageFactory.h

coverage_converter: $(COVERAGE_CONVERTER_OBJS)
	$(CXX) -o $(@) $(COVERAGE_CONVERTER_OBJS)

coverage_converter.o: coverage_converter.cc CoverageMap.h CoverageFactory.h

CoverageFactory.o: CoverageFactory.cc CoverageFactory.h \
  CoverageReaderBase.h CoverageReaderQEMU.h CoverageReaderRTEMS.h \
  CoverageReaderSkyeye.h CoverageReaderTSIM.h  \
  CoverageWriterBase.h CoverageWriterRTEMS.h \
  CoverageWriterSkyeye.h CoverageWriterTSIM.h 
CoverageMap.o: CoverageMap.cc CoverageMap.h
CoverageRanges.o: CoverageRanges.cc CoverageRanges.h
CoverageReaderBase.o: CoverageReaderBase.cc CoverageReaderBase.h
CoverageReaderQEMU.o: CoverageReaderQEMU.cc CoverageReaderQEMU.h qemu-traces.h
CoverageReaderRTEMS.o: CoverageReaderRTEMS.cc CoverageReaderRTEMS.h \
  rtemscov_header.h
CoverageReaderSkyeye.o: CoverageReaderSkyeye.cc CoverageReaderSkyeye.h \
  skyeye_header.h
CoverageReaderTSIM.o: CoverageReaderTSIM.cc CoverageReaderTSIM.h
CoverageWriterBase.o: CoverageWriterBase.cc CoverageWriterBase.h
CoverageWriterRTEMS.o: CoverageWriterRTEMS.cc CoverageWriterRTEMS.h \
  rtemscov_header.h
CoverageWriterSkyeye.o: CoverageWriterSkyeye.cc CoverageWriterSkyeye.h
CoverageWriterTSIM.o: CoverageWriterTSIM.cc CoverageWriterTSIM.h
Explanations.o: Explanations.cc Explanations.h
ObjdumpProcessor.o: ObjdumpProcessor.cc ObjdumpProcessor.h
Reports.o: Reports.cc Reports.h
Toolnames.o: Toolnames.cc Toolnames.h

clean:
	rm -rf $(PROGRAMS) *.o doxy html latex *.exe *~

doxygen:
	doxygen Doxyfile

install: all
