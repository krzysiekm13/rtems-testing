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
#include <string>

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
    CoverageMapBase(
      uint32_t low,
      uint32_t high
    );

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
    virtual void setWasExecuted( uint32_t address );

    /*!
     *  This method return true if the address was executed.
     *
     *  @param[in] address is the address to test if it was executed.
     *  
     *  @return This method returns TRUE if the address was executed
     *  and FALSE if it was not.
     */
    bool wasExecuted( uint32_t address );

    /*!
     *  This methods sets the boolean which indicates if this
     *  is the starting address for an instruction.
     *
     *
     *  @param[in] address is the address for this source line.
     */
    void setIsStartOfInstruction(
      uint32_t    address
    );

    /*!
     *  This methods returns the boolean which indicates if this
     *  is the starting address for an instruction.
     *
     *  @param[in] address is the address for this source line.
     *
     *  @return This methods returns TRUE or FALSE.
     */
    bool isStartOfInstruction( uint32_t address );

    /*!
     *  This methods sets the source line for this address.
     *
     *  @param[in] address is the address for this source line.
     *  @param[in] line is the source line
     */
    void setSourceLine(
      uint32_t    address,
      std::string line
    );

    /*!
     *  This methods returns the source line information for this address.
     *
     *  @param[in] address is the address for this source line.
     *
     *  @return This methods returns the source line associated
     *          with this address.
     */
    std::string sourceLine( uint32_t address );

    // XXX Glenn fix me, need way to set and real logic
    void setIsBranch( uint32_t address );
    bool isBranch( uint32_t address );

    void setWasTaken( uint32_t address );
    void setWasNotTaken( uint32_t address );

    // XXX These require logic I think
    bool wasAlwaysTaken( uint32_t address );
    bool wasNeverTaken( uint32_t address );
  protected:
    /*!
     *  This is the information we can gather and track per address.
     */
    typedef struct {
      bool        wasExecuted;
      bool        isStartOfInstruction;
      bool        isBranch;
      bool        wasTaken;
      bool        wasNotTaken;
      std::string sourceLine;
    } perAddressInfo_t;

    /*!
     *  This is a dynamically allocated array of flags to indicate which
     *  addresses were executed.
     */
    perAddressInfo_t *Info;

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

