/*
 *  $Id$
 */

/*! @file CoverageMapBase.h
 *  @brief CoverageMapBase Specification
 *
 *  This file contains the specification of the CoverageMapBase class.
 */

#ifndef __COVERAGE_MAP_BASE_H__
#define __COVERAGE_MAP_BASE_H__

#include <stdint.h>

namespace Coverage {

  /*! @class CoverageMapBase
   *
   *  This is the base class for Coverage Map implementations.
   */
  class CoverageMapBase {

  public:

    /*! 
     *  This method is the default constructor of a CoverageMapBase instance.
     */
    CoverageMapBase();

    /*! 
     *  This method is the destructor for a CoverageMapBase instance.
     */
    virtual ~CoverageMapBase();

    /*!
     *  This method is invoked to set a flag indicating that the address
     *  was executed.
     *
     *  @param[in] address is the address which was executed.
     */
    virtual void setWasExecuted( uint32_t address ) = 0;

    /*!
     *  This method return true if the address was executed.
     *
     *  @param[in] address is the address to test if it was executed.
     *  
     *  @return This method returns TRUE if the address was executed
     *  and FALSE if it was not.
     */
    virtual bool wasExecuted( uint32_t address ) = 0;
  };

}
#endif

