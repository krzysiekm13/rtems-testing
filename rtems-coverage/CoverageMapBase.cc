
/*
 *  $Id$
 */

/*! @file CoverageMapBase.cc
 *  @brief CoverageMapBase Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a CoverageMap.
 */

#include "CoverageMapBase.h"
#include <stdlib.h>
#include <limits.h>
#include <libgen.h>

namespace Coverage {

  CoverageMapBase::CoverageMapBase(
    uint32_t low,
    uint32_t high
  ) : lowAddress(low), highAddress(high)
  {
    uint32_t a;

    Info = new perAddressInfo_t[ high - low + 1 ];

    for (a=low; a<high; a++) {

      perAddressInfo_t *i = &Info[ a-low ];

      i->wasExecuted          = false;
      i->isStartOfInstruction = false;
      i->isBranch             = false;
      i->wasTaken             = false;
      i->wasNotTaken          = false;
      i->sourceLine.clear();
    }
  }

  CoverageMapBase::~CoverageMapBase()
  {
    if ( Info )
      delete Info;
  }

  bool CoverageMapBase::getBeginningOfInstruction(
    uint32_t  address,
    uint32_t *beginning
  )
  {
    bool     status = false;
    uint32_t start;

    if ((address < lowAddress) || (address > highAddress))
      return status;

    start = address;

    while (start >= lowAddress ) {
      if (Info[ start - lowAddress ].isStartOfInstruction) {
        *beginning = start;
        status = true;
        break;
      }
      else
        start--;
    }

    return status;
  }

  /* XXX Not sure this is what we want */
  uint32_t CoverageMapBase::getBeginningOfNextInstruction(
    uint32_t address
  )
  {
    uint32_t beginning = address++;

    while ((beginning < highAddress ) &&
           (!Info[ beginning - lowAddress ].isStartOfInstruction))
      beginning++;

    return beginning;
  }

  void CoverageMapBase::setIsStartOfInstruction(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].isStartOfInstruction = true;
  }

  bool CoverageMapBase::isStartOfInstruction( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return Info[ address - lowAddress ].isStartOfInstruction;
  }

  void CoverageMapBase::setWasExecuted( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].wasExecuted = true;
  }

  bool CoverageMapBase::wasExecuted( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return Info[ address - lowAddress ].wasExecuted;
  }

  void CoverageMapBase::setSourceLine(
    uint32_t    address,
    std::string line
  )
  {
    char rpath[PATH_MAX];
    char *base;
    uint32_t a;

    if ((address < lowAddress) || (address > highAddress))
      return;

    a = address - lowAddress;
#if 0
    if ( !Info[ a ].isStartOfInstruction ) {
      find start of this instuction
      set a to start of this instruction
    }
#endif
    
    // obtain the full clean absolute path 
    realpath( line.c_str(), rpath );
    // XXX TDB rip off up until cpukit .. want cpukit/score/src/chain.c

    // Get the filename (no directory)
    base = basename( rpath );
    Info[ a ].sourceLine = std::string( base );
  }

  std::string CoverageMapBase::getSourceLine( uint32_t address )
  {
    uint32_t a;

    if ( address < lowAddress )
      return "OUT_OF_RANGE_LOW";
    if ( address > highAddress )
      return "OUT_OF_RANGE_HIGH";

    a = address - lowAddress;
#if 0
    if ( !Info[ a ].isStartOfInstruction ) {
      find start of this instuction
      set a to start of this instruction
    }
#endif
    return Info[ a ].sourceLine;
  }

  void CoverageMapBase::setIsBranch(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].isBranch = true;
  }

  bool CoverageMapBase::isBranch( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return Info[ address - lowAddress ].isBranch;
  }

  void CoverageMapBase::setWasTaken(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].wasTaken = true;
  }

  void CoverageMapBase::setWasNotTaken(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].wasNotTaken = true;
  }

  bool CoverageMapBase::wasAlwaysTaken( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return (Info[ address - lowAddress ].wasTaken &&
            !Info[ address - lowAddress ].wasNotTaken);
  }

  bool CoverageMapBase::wasNeverTaken( uint32_t address )
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return (!Info[ address - lowAddress ].wasTaken);
  }
}
