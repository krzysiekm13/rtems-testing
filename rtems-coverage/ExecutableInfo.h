/*
 *  $Id$
 */

/*! @file ExecutableInfo.h
 *  @brief ExecutableInfo Specification
 *
 *  This file contains the specification of the ExecutableInfo class.
 */

#ifndef __EXECUTABLEINFO_H__
#define __EXECUTABLEINFO_H__

#include <map>
#include <stdint.h>
#include <string>

#include "CoverageMapBase.h"
#include "SymbolTable.h"

namespace Coverage {

  /*! @class ExecutableInfo
   *
   *  This class holds a collection of information for an executable
   *  that is to be analyzed.
   */
  class ExecutableInfo {

  public:

    /*!
     *  This method constructs an ExecutableInfo instance.
     *
     *  @param[in] executableName specifies the filename of the executable
     */
    ExecutableInfo( const char* const executableName );

    /*!
     *  This method destructs an ExecutableInfo instance.
     */
    virtual ~ExecutableInfo();

    /*!
     *  This method prints the contents of all coverage maps for
     *  this executable.
     */
    void dumpCoverageMaps( void );

    /*!
     *  This method returns a pointer to the executable's coverage map
     *  that contains the specified address.
     *
     *  @param[in] address specifies the desired address
     *
     *  @return Returns a pointer to the coverage map
     */
    CoverageMapBase* getCoverageMap( uint32_t address );

    /*!
     *  This method returns the file name of the executable.
     *
     *  @return Returns the executable's file name
     */
    std::string getFileName( void ) const;

    /*!
     *  This method returns a pointer to the executable's symbol table.
     *
     *  @return Returns a pointer to the symbol table.
     */
    SymbolTable* getSymbolTable( void ) const;

    /*!
     *  This method initializes the ExecutableInfo instance.
     */
    void initialize( void );

    /*!
     *  This method marks the specified address of the executable
     *  as a branch instruction and indicates that it was NOT taken.
     *
     *  @param[in] address indicates the address of the branch instruction
     *             that was NOT taken
     */
    void markBranchNotTaken(
      uint32_t address
    );

    /*!
     *  This method marks the specified address of the executable
     *  as a branch instruction and indicates that it was taken.
     *
     *  @param[in] address indicates the address of the branch instruction
     *             that was taken
     */
    void markBranchTaken(
      uint32_t address
    );

    /*!
     *  This method marks the specified address of the executable
     *  as the starting address for an instruction.
     *
     *  @param[in] address indicates the address of the start of an instruction
     */
    void markStartOfInstruction(
      uint32_t address
    );

    /*!
     *  This method marks the specified address of the executable
     *  as an address that was executed.
     *
     *  @param[in] address indicates the address that was executed
     */
    void markWasExecuted(
      uint32_t address
    );

    /*!
     *  This method merges the coverage maps for this executable into
     *  the unified coverage map.
     */
    void mergeCoverage( void );

  private:

    /*!
     *  This map associates a symbol with its coverage map.
     */
    typedef std::map<std::string, CoverageMapBase *> coverageMaps_t;
    coverageMaps_t coverageMaps;

    /*!
     *  This member variable contains the filename of the executable.
     */
    std::string fileName;

    /*!
     *  This member variable contains a pointer to the symbol table
     *  of the executable.
     */
    SymbolTable* theSymbolTable;

  };
}
#endif
