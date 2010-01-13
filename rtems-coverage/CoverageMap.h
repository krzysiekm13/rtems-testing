/*
 *  $Id$
 */

/*! @file CoverageMap.h
 *  @brief CoverageMap Specification
 *
 *  This file contains the specification of the CoverageMap class.
 */

#ifndef __COVERAGE_MAP_H__
#define __COVERAGE_MAP_H__

#include "CoverageMapBase.h"

namespace Coverage {

  /*! @class CoverageMap
   *
   *  This class implements a coverage map which only supports a single
   *  range of addresses -- low to high. 
   */
  class CoverageMap : public CoverageMapBase {

  public:

    /*! 
     *  This method is the constructor of a CoverageMap instance.
     *
     *  @param[in] low is the low address of the range.
     *  @param[in] high is the high address of the range.
     */
    CoverageMap(
      uint32_t low,
      uint32_t high
    );

    /* Inherit documentation from base class. */
    virtual ~CoverageMap();

  private:

  };

}
#endif

