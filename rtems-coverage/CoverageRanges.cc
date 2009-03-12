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

namespace Coverage {

  CoverageRanges::CoverageRanges()
  {
  }

  CoverageRanges::~CoverageRanges()
  {
  }

  void CoverageRanges::add(
    uint32_t lowAddressArg,
    uint32_t highAddressArg
  )
  {
    CoverageRange c;

    c.lowAddress  = lowAddressArg;
    c.highAddress = highAddressArg;
    Set.push_back(c);
  }
}
