/*
 *   $Id$
 */

#ifndef __APP_COMMON_h
#define __APP_COMMON_h

#include "CoverageReaderBase.h"
#include "CoverageWriterBase.h"
#include "Explanations.h"
#include "CoverageRanges.h"
#include "ObjdumpProcessor.h"
#include "Toolnames.h"
#include <list>

extern Coverage::CoverageMapBase    *CoverageMap;
extern Coverage::CoverageReaderBase *CoverageReader;
extern Coverage::CoverageWriterBase *CoverageWriter;
extern Coverage::Explanations       *Explanations;
extern Coverage::ObjdumpProcessor   *ObjdumpProcessor;
extern Coverage::CoverageRanges     *Ranges;
extern Coverage::Toolnames          *Tools;
extern std::list<uint32_t>           AddressesNeedingSourceLine; 
extern int  BranchesAlwaysTaken;
extern bool BranchesFound;
extern int  BranchesNeverTaken;
extern int  UncoveredRanges;

#endif
