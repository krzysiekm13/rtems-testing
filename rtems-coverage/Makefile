#
# $Id$
#

INSTALL_DIR=../bin
CXXFLAGS=-g -Wall -O2 -march=native -mtune=native
PROGRAMS=covoar qemu-dump-trace

COVOAR_OBJS= app_common.o \
  covoar.o \
  CoverageFactory.o \
  CoverageMap.o \
  CoverageMapBase.o \
  CoverageRanges.o \
  CoverageReaderBase.o \
  CoverageReaderQEMU.o \
  CoverageReaderRTEMS.o \
  CoverageReaderSkyeye.o \
  CoverageReaderTSIM.o \
  CoverageWriterBase.o \
  CoverageWriterRTEMS.o \
  CoverageWriterSkyeye.o \
  CoverageWriterTSIM.o \
  DesiredSymbols.o \
  ExecutableInfo.o \
  Explanations.o \
  ObjdumpProcessor.o \
  Reports.o \
  SymbolTable.o \
  Toolnames.o

all: $(PROGRAMS) ${INSTALL_DIR} ../bin/qemu-dump-trace

../bin/qemu-dump-trace: qemu-dump-trace ${INSTALL_DIR}
	cp qemu-dump-trace ../bin

${INSTALL_DIR}:
	test -d ${INSTALL_DIR} || mkdir ${INSTALL_DIR}

qemu-dump-trace: qemu-dump-trace.c ${INSTALL_DIR}
	$(CXX) -o $(@) qemu-dump-trace.c

covoar: $(COVOAR_OBJS)
	$(CXX) $(CXXFLAGS) -o $(@) $(COVOAR_OBJS)

app_common.o: app_common.h app_common.cc

covoar.o: covoar.cc CoverageFactory.h CoverageMap.h DesiredSymbols.h \
  ExecutableInfo.h Explanations.h ObjdumpProcessor.h Reports.h

CoverageFactory.o: CoverageFactory.cc CoverageFactory.h \
  CoverageReaderBase.h CoverageReaderQEMU.h CoverageReaderRTEMS.h \
  CoverageReaderSkyeye.h CoverageReaderTSIM.h  \
  CoverageWriterBase.h CoverageWriterRTEMS.h \
  CoverageWriterSkyeye.h CoverageWriterTSIM.h 
CoverageMap.o: CoverageMap.cc CoverageMap.h
CoverageMapBase.o: CoverageMapBase.cc CoverageMapBase.h
CoverageRanges.o: CoverageRanges.cc CoverageRanges.h
CoverageReaderBase.o: CoverageReaderBase.cc CoverageReaderBase.h
CoverageReaderQEMU.o: CoverageReaderQEMU.cc CoverageReaderQEMU.h \
  ExecutableInfo.h qemu-traces.h
CoverageReaderRTEMS.o: CoverageReaderRTEMS.cc CoverageReaderRTEMS.h \
  ExecutableInfo.h rtemscov_header.h
CoverageReaderSkyeye.o: CoverageReaderSkyeye.cc CoverageReaderSkyeye.h \
  ExecutableInfo.h skyeye_header.h
CoverageReaderTSIM.o: CoverageReaderTSIM.cc CoverageReaderTSIM.h \
  ExecutableInfo.h
CoverageWriterBase.o: CoverageWriterBase.cc CoverageWriterBase.h
CoverageWriterRTEMS.o: CoverageWriterRTEMS.cc CoverageWriterRTEMS.h \
  rtemscov_header.h
CoverageWriterSkyeye.o: CoverageWriterSkyeye.cc CoverageWriterSkyeye.h \
  skyeye_header.h
CoverageWriterTSIM.o: CoverageWriterTSIM.cc CoverageWriterTSIM.h
DesiredSymbols.o: DesiredSymbols.cc DesiredSymbols.h CoverageMap.h
ExecutableInfo.o: ExecutableInfo.cc ExecutableInfo.h CoverageMap.h \
  DesiredSymbols.h SymbolTable.h
Explanations.o: Explanations.cc Explanations.h
ObjdumpProcessor.o: ObjdumpProcessor.cc ObjdumpProcessor.h ExecutableInfo.h
Reports.o: Reports.cc Reports.h CoverageRanges.h DesiredSymbols.h \
  Explanations.h ObjdumpProcessor.h
SymbolTable.o: SymbolTable.cc SymbolTable.h
Toolnames.o: Toolnames.cc Toolnames.h

clean:
	rm -rf $(PROGRAMS) *.o doxy html latex *.exe *~

doxygen:
	doxygen Doxyfile

install: all
