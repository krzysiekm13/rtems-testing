
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

namespace Coverage {

  CoverageMapBase::CoverageMapBase(
    uint32_t low,
    uint32_t high
  ) : lowAddress(low),
      highAddress(high)
  {
    uint32_t a;

    Info = new perAddressInfo_t[ high - low + 1 ];

    for ( a=low ; a<high ; a++ ) {
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

  void CoverageMapBase::setWasExecuted( uint32_t address )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].wasExecuted = true;
  }

  bool CoverageMapBase::wasExecuted( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    return Info[ address - lowAddress ].wasExecuted;
  }

  void CoverageMapBase::setIsStartOfInstruction(
    uint32_t    address
  )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].isStartOfInstruction = true;
  }

  bool CoverageMapBase::isStartOfInstruction( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    return Info[ address - lowAddress ].isStartOfInstruction;
  }

  void CoverageMapBase::setSourceLine(
    uint32_t    address,
    std::string line
  )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].sourceLine = line;
  }

  std::string CoverageMapBase::sourceLine( uint32_t address )
  {
    if ( address < lowAddress )
      return "OUT_OF_RANGE_LOW";
    if ( address > highAddress )
      return "OUT_OF_RANGE_HIGH";
    return Info[ address - lowAddress ].sourceLine;
  }

  void CoverageMapBase::setIsBranch(
    uint32_t    address
  )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].isBranch = true;
  }

  bool CoverageMapBase::isBranch( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    return false;
  }

  void CoverageMapBase::setWasNotTaken(
    uint32_t    address
  )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].wasNotTaken = true;
  }

  void CoverageMapBase::setWasTaken(
    uint32_t    address
  )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    Info[ address - lowAddress ].wasTaken = true;
  }

  bool CoverageMapBase::wasAlwaysTaken( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    // XXX TBD this will take some logic to be right (I think)
    return false;
  }

  bool CoverageMapBase::wasNeverTaken( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    // XXX TBD this will take some logic to be right (I think)
    return false;
  }
}
