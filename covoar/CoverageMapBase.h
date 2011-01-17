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
     *  This method constructs a CoverageMapBase instance.
     *
     *  @param[in] low specifies the lowest address of the coverage map
     *  @param[in] high specifies the highest address of the coverage map
     */
    CoverageMapBase(
      uint32_t low,
      uint32_t high
    );

    /*! 
     *  This method destructs a CoverageMapBase instance.
     */
    virtual ~CoverageMapBase();

    /*!
     *  This method prints the contents of the coverage map to stdout.
     */
    void dump( void ) const;

    /*!
     *  This method returns the address of the beginning of the
     *  instruction that contains the specified address.
     *
     *  @param[in] address specifies the address to search from
     *  @param[out] beginning contains the address of the beginning of
     *              the instruction.
     *  
     *  @return Returns TRUE if the beginning of the instruction was
     *   found and FALSE if it was not.
     */
    bool getBeginningOfInstruction(
      uint32_t  address,
      uint32_t* beginning
    ) const;

    /*!
     *  This method returns the high address of the coverage map.
     *
     *  @return Returns the high address of the coverage map.
     */
    uint32_t getHighAddress( void ) const;

    /*!
     *  This method returns the low address of the coverage map.
     *
     *  @return Returns the low address of the coverage map.
     */
    uint32_t getLowAddress( void ) const;

    /*!
     *  This method sets the boolean which indicates if this
     *  is the starting address for an instruction.
     *
     *  @param[in] address specifies the address of the start of an instruction
     */
    void setIsStartOfInstruction(
      uint32_t address
    );

    /*!
     *  This method returns a boolean which indicates if this
     *  is the starting address of an instruction.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if the specified address is the start
     *   of an instruction and FALSE otherwise.
     */
    bool isStartOfInstruction( uint32_t address ) const;

    /*!
     *  This method sets the boolean which indicates that the instruction
     *  at the specified address was executed.
     *
     *  @param[in] address specifies the address which was executed
     */
    virtual void setWasExecuted( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the instruction
     *  at the specified address was executed.
     *
     *  @param[in] address specifies the address to check
     *  
     *  @return Returns TRUE if the instruction at the specified
     *   address was executed and FALSE otherwise.
     */
    bool wasExecuted( uint32_t address ) const;

    /*!
     *  This method sets the boolean which indicates if the specified
     *  address is the starting address of a branch instruction.
     *
     *  @param[in] address specifies the address of the branch instruction
     */
    void setIsBranch( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the specified
     *  address is the starting address of a NOP instruction.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if a NOP instruction is at the
     *   specified address and FALSE otherwise.
     */
    bool isNop( uint32_t address ) const;

    /*!
     *  This method sets the boolean which indicates if the specified
     *  address is the starting address of a NOP instruction.
     *
     *  @param[in] address specifies the address of the NOP instruction
     */
    void setIsNop( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the specified
     *  address is the starting address of a branch instruction.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if a branch instruction is at the
     *   specified address and FALSE otherwise.
     */
    bool isBranch( uint32_t address ) const;

    /*!
     *  This method sets the boolean which indicates if the branch
     *  at the specified address was taken.
     *
     *  @param[in] address specifies the address of the branch instruction
     */
    void setWasTaken( uint32_t address );

    /*!
     *  This method sets the boolean which indicates if the branch
     *  at the specified address was NOT taken.
     *
     *  @param[in] address specifies the address of the branch instruction
     */
    void setWasNotTaken( uint32_t address );

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address is ALWAYS taken.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if the branch instruction at the
     *   specified address is ALWAYS taken and FALSE otherwise.
     */
    bool wasAlwaysTaken( uint32_t address ) const;

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address is NEVER taken.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if the branch instruction at the
     *  specified address is NEVER taken and FALSE otherwise.
     */
    bool wasNeverTaken( uint32_t address ) const;

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address was NOT taken.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if the branch instruction at the
     *   specified address was NOT taken and FALSE otherwise.
     */
    bool wasNotTaken( uint32_t address ) const;

    /*!
     *  This method returns a boolean which indicates if the branch
     *  instruction at the specified address was taken.
     *
     *  @param[in] address specifies the address to check
     *
     *  @return Returns TRUE if the branch instruction at the
     *  specified address was taken and FALSE otherwise.
     */
    bool wasTaken( uint32_t address ) const;

  protected:

    /*!
     *  This structure defines the information that is gathered and
     *  tracked per address.
     */
    typedef struct {
      /*!
       *  This member indicates that the address is the start of
       *  an instruction.
       */
      bool isStartOfInstruction;
      /*!
       *  This member indicates that the address was executed.
       */
      bool wasExecuted;
      /*!
       *  This member indicates that the address is a branch instruction.
       */
      bool isBranch;
      /*!
       *  This member indicates that the address is a NOP instruction.
       */
      bool isNop;
      /*!
       *  When isBranch is TRUE, this member indicates that the branch
       *  instruction at the address was taken.
       */
      bool wasTaken;
      /*!
       *  When isBranch is TRUE, this member indicates that the branch
       *  instruction at the address was NOT taken.
       */
      bool wasNotTaken;
    } perAddressInfo_t;

    /*!
     *  This is a dynamically allocated array of data that is
     *  kept for each address.
     */
    perAddressInfo_t* Info;

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
