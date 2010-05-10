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

  /*! @class CoverageRanges
   *
   *  This class defines a set of address ranges for which coverage
   *  did not occur.  Each address range can either define a range of
   *  bytes that was not executed or a range of bytes for a branch
   *  instruction that was not completely covered (i.e. taken and NOT
   *  taken).
   */
  class CoverageRanges {

  public:

    /*!
     *  This type defines the reasons to associate with a range.
     */
    typedef enum {
      UNCOVERED_REASON_NOT_EXECUTED,
      UNCOVERED_REASON_BRANCH_ALWAYS_TAKEN,
      UNCOVERED_REASON_BRANCH_NEVER_TAKEN
    } uncoveredReason_t;

    /*!
     *  This type defines the information kept for each range.
     */
    typedef struct {
      uint32_t          id;
      uint32_t          lowAddress;
      std::string       lowSourceLine;
      uint32_t          highAddress;
      std::string       highSourceLine;
      uncoveredReason_t reason;
    } coverageRange_t;

    /*!
     *  This member variable contains a list of CoverageRange instances.
     */
    typedef std::list<coverageRange_t> ranges_t;
    ranges_t set;

    /*! 
     *  This method constructs a CoverageRanges instance.
     */
    CoverageRanges();

    /*! 
     *  This method destructs a CoverageRanges instance.
     */
    ~CoverageRanges();

    /*!
     *  This method adds a range entry to the set of ranges.
     *
     *  @param[in] lowAddressArg specifies the lowest address of the range
     *  @param[in] highAddressArg specifies the highest address of the range
     *  @param[in] why specifies the reason that the range was added
     *
     */
    void add(
      uint32_t          lowAddressArg,
      uint32_t          highAddressArg,
      uncoveredReason_t why
    );
 

    /*!
     *  This method returns the index of a range given the low address.
     *  Upon failure on finding the adress 0 is returned.
     */
    uint32_t getId( uint32_t lowAddress );
 
    protected:

      /*!
       *  This member variable tracks a unique index for the ranges_t block.
       */
      uint32_t  id_m;

  };

}
#endif
