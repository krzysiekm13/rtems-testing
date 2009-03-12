

CXXFLAGS=-g -Wall -O2
PROGRAMS=covmerge
COVMERGE_OBJS= covmerge.o CoverageMap.o CoverageMapBase.o \
  CoverageRanges.o \
  CoverageReaderBase.o CoverageReaderTSIM.o \
  CoverageWriterBase.o CoverageWriterTSIM.o \
  Explanations.o ObjdumpProcessor.o Toolnames.o

all: $(PROGRAMS)

covmerge: $(COVMERGE_OBJS)
	$(CXX) -o $(@) $(COVMERGE_OBJS)

covmerge.o: covmerge.cc CoverageMap.h CoverageReaderBase.h \
  CoverageReaderNops.h CoverageReaderTSIM.h CoverageWriterBase.h \
  CoverageWriterTSIM.h

CoverageMap.o: CoverageMap.cc CoverageMap.h
CoverageRanges.o: CoverageRanges.cc CoverageRanges.h
CoverageReaderBase.o: CoverageReaderBase.cc CoverageReaderBase.h
CoverageReaderTSIM.o: CoverageReaderTSIM.cc CoverageReaderTSIM.h
CoverageWriterBase.o: CoverageWriterBase.cc CoverageWriterBase.h
CoverageWriterTSIM.o: CoverageWriterTSIM.cc CoverageWriterTSIM.h
Explanations.o: Explanations.cc Explanations.h
ObjdumpProcessor.o: ObjdumpProcessor.cc ObjdumpProcessor.h
Toolnames.o: Toolnames.cc Toolnames.h


clean:
	rm -rf $(PROGRAMS) *.o doxy html latex *.exe *~

doxygen:
	doxygen Doxyfile

