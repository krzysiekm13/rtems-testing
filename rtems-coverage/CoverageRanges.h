/*
 *  $Id$
 */

/*! @file CoverageRanges.h
 *  @brief CoverageRanges Specification
 *
 *  This file contains the specification of the CoverageRanges class.
 */

#ifndef __COVERAGE_RANGES_H__
#define __COVERAGE_RANGES_H__

#include <stdint.h>
#include <list>
#include <string>

namespace Coverage {

  class CoverageRange {
  public:

    uint32_t lowAddress;
    uint32_t highAddress;

    CoverageRange() {}
    CoverageRange(
      uint32_t lowAddressArg,
      uint32_t highAddressArg
    ) :
      lowAddress(lowAddressArg),
      highAddress(highAddressArg)
    {
    }

    ~CoverageRange() {}
  };

  /*! @class CoverageRanges
   *
   *  This is the base class for Coverage Map implementations.
   */
  class CoverageRanges {

  public:
    /*!
     */
    std::list<CoverageRange> Set;

    /*! 
     *  This method is the default constructor of a CoverageRanges instance.
     */
    CoverageRanges();

    /*! 
     *  This method is the destructor for a CoverageRanges instance.
     */
    ~CoverageRanges();

    /*!
     */
    void add(
      uint32_t lowAddressArg,
      uint32_t highAddressArg
    );

  };

}
#endif

