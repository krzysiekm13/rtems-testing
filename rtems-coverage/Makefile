#
# $Id$
#

CXXFLAGS=-g -Wall -O2
PROGRAMS=covmerge trace_converter
SHARED_OBJS=CoverageMap.o CoverageMapBase.o \
  CoverageFactory.o \
  CoverageReaderBase.o CoverageReaderRTEMS.o \
  CoverageReaderSkyeye.o CoverageReaderTSIM.o \
  CoverageWriterBase.o CoverageWriterRTEMS.o \
  CoverageWriterSkyeye.o CoverageWriterTSIM.o

COVMERGE_OBJS= covmerge.o $(SHARED_OBJS) \
  CoverageRanges.o \
  Explanations.o ObjdumpProcessor.o Toolnames.o

TRACE_CONVERTER_OBJS=trace_converter.o $(SHARED_OBJS) \
  TraceReaderBase.o TraceFactory.o

all: $(PROGRAMS)

covmerge: $(COVMERGE_OBJS)
	$(CXX) -o $(@) $(COVMERGE_OBJS)

covmerge.o: covmerge.cc CoverageMap.h CoverageFactory.h

trace_converter: $(TRACE_CONVERTER_OBJS)
	$(CXX) -o $(@) $(TRACE_CONVERTER_OBJS)

trace_converter.o: trace_converter.cc CoverageMap.h CoverageFactory.h \
       TraceFactory.h

CoverageFactory.o: CoverageFactory.cc CoverageFactory.h CoverageReaderBase.h \
  CoverageReaderNops.h CoverageReaderSkyeye.h CoverageReaderTSIM.h  \
  CoverageWriterBase.h CoverageWriterSkyeye.h CoverageWriterTSIM.h 
CoverageMap.o: CoverageMap.cc CoverageMap.h
CoverageRanges.o: CoverageRanges.cc CoverageRanges.h
CoverageReaderBase.o: CoverageReaderBase.cc CoverageReaderBase.h
CoverageReaderSkyeye.o: CoverageReaderSkyeye.cc CoverageReaderSkyeye.h \
  skyeye_header.h
CoverageReaderTSIM.o: CoverageReaderTSIM.cc CoverageReaderTSIM.h \
  skyeye_header.h
CoverageWriterBase.o: CoverageWriterBase.cc CoverageWriterBase.h
CoverageWriterSkyeye.o: CoverageWriterSkyeye.cc CoverageWriterSkyeye.h
CoverageWriterTSIM.o: CoverageWriterTSIM.cc CoverageWriterTSIM.h
Explanations.o: Explanations.cc Explanations.h
ObjdumpProcessor.o: ObjdumpProcessor.cc ObjdumpProcessor.h
Toolnames.o: Toolnames.cc Toolnames.h


clean:
	rm -rf $(PROGRAMS) *.o doxy html latex *.exe *~

doxygen:
	doxygen Doxyfile

