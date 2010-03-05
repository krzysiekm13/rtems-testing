
/*
 *  $Id$
 */

/*! @file CoverageMapBase.cc
 *  @brief CoverageMapBase Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a CoverageMap.
 */

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "CoverageMapBase.h"

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

      i->isStartOfInstruction = false;
      i->wasExecuted          = false;
      i->isBranch             = false;
      i->wasTaken             = false;
      i->wasNotTaken          = false;
    }
  }

  CoverageMapBase::~CoverageMapBase()
  {
    if (Info)
      delete Info;
  }

  void CoverageMapBase::dump( void ) const {

    uint32_t          a;
    perAddressInfo_t* entry;

    fprintf( stderr, "Coverage Map Contents:\n" );

    for (a = lowAddress; a <= highAddress; a++) {

      entry = &Info[ a - lowAddress ];

      fprintf(
        stderr,
        "0x%x - isStartOfInstruction = %s, wasExecuted = %s\n",
        a,
        entry->isStartOfInstruction ? "TRUE" : "FALSE",
        entry->wasExecuted ? "TRUE" : "FALSE"
      );
      fprintf(
        stderr,
        "           isBranch = %s, wasTaken = %s, wasNotTaken = %s\n",
        entry->isBranch ? "TRUE" : "FALSE",
        entry->wasTaken ? "TRUE" : "FALSE",
        entry->wasNotTaken ? "TRUE" : "FALSE"
      );
    }
  }

  bool CoverageMapBase::getBeginningOfInstruction(
    uint32_t  address,
    uint32_t* beginning
  ) const
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

  uint32_t CoverageMapBase::getHighAddress( void ) const
  {
    return highAddress;
  }

  uint32_t CoverageMapBase::getLowAddress( void ) const
  {
    return lowAddress;
  }

  void CoverageMapBase::setIsStartOfInstruction(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].isStartOfInstruction = true;
  }

  bool CoverageMapBase::isStartOfInstruction( uint32_t address ) const
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

  bool CoverageMapBase::wasExecuted( uint32_t address ) const
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return Info[ address - lowAddress ].wasExecuted;
  }

  void CoverageMapBase::setIsBranch(
    uint32_t    address
  )
  {
    if ((address < lowAddress) || (address > highAddress))
      return;
    Info[ address - lowAddress ].isBranch = true;
  }

  bool CoverageMapBase::isBranch( uint32_t address ) const
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

  bool CoverageMapBase::wasAlwaysTaken( uint32_t address ) const
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return (Info[ address - lowAddress ].wasTaken &&
            !Info[ address - lowAddress ].wasNotTaken);
  }

  bool CoverageMapBase::wasNeverTaken( uint32_t address ) const
  {
    if ((address < lowAddress) || (address > highAddress))
      return false;
    return (!Info[ address - lowAddress ].wasTaken);
  }

  bool CoverageMapBase::wasNotTaken( uint32_t address ) const
  {
    return (Info[ address - lowAddress ].wasNotTaken);
  }

  bool CoverageMapBase::wasTaken( uint32_t address ) const
  {
    return (Info[ address - lowAddress ].wasTaken);
  }
}
