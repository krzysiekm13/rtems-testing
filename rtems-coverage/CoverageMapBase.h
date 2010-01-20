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
     *  This method returns the address of the beginning of the
     *  instruction that contains the specified address.
     *
     *  @param[in] address is the address to search from
     *  @param[out] beginning contains the address of the beginning of
     *  the instruction.
     *  
     *  @return This method returns TRUE if the beginning of the instruction
     *  was found and FALSE if it was not.
     */
    bool getBeginningOfInstruction(
      uint32_t  address,
      uint32_t *beginning
    );

    /*!
     *  This method returns the address of the beginning of the
     *  next instruction that follows the specified address.
     *
     *  @param[in] address is the address to search from
     *  
     *  @return This method returns the address of the beginning of the
     *  instruction.
     */
    uint32_t getBeginningOfNextInstruction(
      uint32_t address
    );

    /*!
     *  This method sets the boolean which indicates if this
     *  is the starting address for an instruction.
     *
     *  @param[in] address is the address of the start of an instruction
     */
    void setIsStartOfInstruction(
      uint32_t    address
    );

    /*!
     *  This method returns a boolean which indicates if this
     *  is the starting address of an instruction.
     *
     *  @param[in] address is the address to check
     *
     *  @return This method returns TRUE if the specified address is
     *  the start of an instruction and FALSE otherwise.
     */
    bool isStartOfInstruction( uint32_t address );

    /*!
     *  This method sets the boolean which indicates that the instruction
     *  at the specified address was executed.
     *
     *  @param[in] address is the address which was executed
     */
    virtual void setWasExecuted( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the instruction
     *  at the specified address was executed.
     *
     *  @param[in] address is the address to check
     *  
     *  @return This method returns TRUE if the instruction at the
     *  specified address was executed and FALSE otherwise.
     */
    bool wasExecuted( uint32_t address );

    /*!
     *  This method sets the source line associated with the specified
     *  address.
     *
     *  @param[in] address is the address to associate with this source line
     *  @param[in] line is the source line
     */
    void setSourceLine(
      uint32_t    address,
      std::string line
    );

    /*!
     *  This method returns the source line associated with the specified
     *  address.
     *
     *  @param[in] address is the address for which to find the source line
     *
     *  @return This method returns the source line associated
     *          with the specified address.
     */
    std::string getSourceLine( uint32_t address );

    /*!
     *  This method sets the boolean which indicates if the specified
     *  address is the starting address of a branch instruction.
     *
     *  @param[in] address is the address of the branch instruction
     */
    void setIsBranch( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the specified
     *  address is the starting address of a branch instruction.
     *
     *  @param[in] address is the address to check
     *
     *  @return This method returns TRUE if a branch instruction is
     *  at the specified address and FALSE otherwise.
     */
    bool isBranch( uint32_t address );

    /*!
     *  This method sets the boolean which indicates if the branch
     *  at the specified address was taken.
     *
     *  @param[in] address is the address of the branch instruction
     */
    void setWasTaken( uint32_t address );

    /*!
     *  This method sets the boolean which indicates if the branch
     *  at the specified address was NOT taken.
     *
     *  @param[in] address is the address of the branch instruction
     */
    void setWasNotTaken( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address is always taken.
     *
     *  @param[in] address is the address to check
     *
     *  @return This method returns TRUE if the branch instruction at
     *  the specified address is always taken and FALSE otherwise.
     */
    bool wasAlwaysTaken( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address is never taken.
     *
     *  @param[in] address is the address to check
     *
     *  @return This method returns TRUE if the branch instruction at
     *  the specified address is never taken and FALSE otherwise.
     */
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
     *  This is the low address of the address map range.
     */
    uint32_t lowAddress;

    /*!
     *  This is the high address of the address map range.
     */
    uint32_t highAddress;

  };

}
#endif
