/*
 *   $Id$
 */

#ifndef __APP_COMMON_h
#define __APP_COMMON_h

#include "CoverageReaderBase.h"
#include "CoverageWriterBase.h"
#include "Toolnames.h"
#include <list>

extern Coverage::CoverageMapBase    *CoverageMap;
extern Coverage::CoverageReaderBase *CoverageReader;
extern Coverage::CoverageWriterBase *CoverageWriter;
extern Coverage::Toolnames          *Tools;
extern std::list<uint32_t>           AddressesNeedingSourceLine; 

#endif
