/*
 *  $Id$
 */

/*! @file CoverageMap.cc
 *  @brief CoverageMap Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */

#include "CoverageMap.h"
#include <stdlib.h>

namespace Coverage {

  CoverageMap::CoverageMap(
    uint32_t low,
    uint32_t high
  ) : lowAddress(low),
      highAddress(high)
  {
    wasExecutedArray = (bool *)calloc( 1, high - low );
  }

  CoverageMap::~CoverageMap()
  {
    if ( wasExecutedArray )
      free( wasExecutedArray );
  }

  void CoverageMap::setWasExecuted( uint32_t address )
  {
    if ( address < lowAddress )
      return;
    if ( address > highAddress )
      return;
    wasExecutedArray[ address - lowAddress ] = true;
  }

  bool CoverageMap::wasExecuted( uint32_t address )
  {
    if ( address < lowAddress )
      return false;
    if ( address > highAddress )
      return false;
    return wasExecutedArray[ address - lowAddress ];
  }
}
