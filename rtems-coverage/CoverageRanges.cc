/*
 *  $Id$
 */

/*! @file CoverageRanges.cc
 *  @brief CoverageRanges Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a CoverageRanges.
 */

#include "CoverageRanges.h"
#include <stdio.h>

namespace Coverage {

  CoverageRanges::CoverageRanges()
  {
    id_m = 0;
  }

  CoverageRanges::~CoverageRanges()
  {
  }

  void CoverageRanges::add(
    uint32_t          lowAddressArg,
    uint32_t          highAddressArg,
    uncoveredReason_t why
  )
  {
    coverageRange_t c;

    id_m++;
    c.id          = id_m;
    c.lowAddress  = lowAddressArg;
    c.highAddress = highAddressArg;
    c.reason      = why;
    set.push_back(c);
  }

  uint32_t  CoverageRanges::getId( uint32_t lowAddress )
  {
    Coverage::CoverageRanges::ranges_t::iterator    ritr;
    uint32_t                                        result = 0;

    for (ritr =  set.begin() ;
         ritr != set.end() ;
         ritr++ ) {
      if ( ritr->lowAddress == lowAddress ) {
        result = ritr->id;
        break;
      }
    }

    return result;
  }

}
