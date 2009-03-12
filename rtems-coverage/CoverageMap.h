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

    /* Inherit documentation from base class. */
    void setWasExecuted( uint32_t address );

    /* Inherit documentation from base class. */
    bool wasExecuted( uint32_t address );

  private:

    /*!
     *  This is a dynamically allocated array of flags to indicate which
     *  addresses were executed.
     */
    bool *wasExecutedArray;

    /*!
     *  This is the low address of the address range being analyzed.
     */
    uint32_t lowAddress;

    /*!
     *  This is the high address of the address range being analyzed.
     */
    uint32_t highAddress;

  };

}
#endif

