/*
 *  $Id$
 */

/*! @file DesiredSymbols.h
 *  @brief DesiredSymbols Specification
 *
 *  This file contains the specification of the DesiredSymbols class.
 */

#ifndef __DESIRED_SYMBOLS_H__
#define __DESIRED_SYMBOLS_H__

#include <list>
#include <map>
#include <stdint.h>
#include <string>

#include "CoverageMapBase.h"
#include "CoverageRanges.h"
#include "ObjdumpProcessor.h"

namespace Coverage {

  /*! @class SymbolInformation
   *
   *  This class defines the information kept for each symbol that is
   *  to be analyzed.
   */
  class SymbolInformation {

  public:

    /*!
     *  This member contains the base address of the symbol.
     */
    uint32_t baseAddress;

    /*!
     *  This member contains the size of the symbol.
     */
    uint32_t sizeInBytes;

    /*!
     *  This member contains the disassembly associated with a symbol.
     */
    std::list<ObjdumpProcessor::objdumpLine_t> instructions;

    /*!
     *  This member contains the name of the file that was used to
     *  generate the disassembled instructions.
     */
    std::string sourceFile;

    /*!
     *  This member contains information about the branch instructions of
     *  a symbol that were not fully covered (i.e. taken/not taken).
     */
    CoverageRanges* uncoveredBranches;

    /*!
     *  This member contains information about the instructions of a
     *  symbol that were not executed.
     */
    CoverageRanges* uncoveredRanges;

    /*!
     *  This member contains the unified or merged coverage map
     *  for the symbol.
     */
    CoverageMapBase* unifiedCoverageMap;

    /*!
     *  This method constructs a SymbolInformation instance.
     */
    SymbolInformation() :
      baseAddress( 0 ),
      sizeInBytes( 0 ),
      uncoveredBranches( NULL ),
      uncoveredRanges( NULL ),
      unifiedCoverageMap( NULL )
    {
    }

    ~SymbolInformation() {}
  };

  /*! @class DesiredSymbols
   *
   *  This class defines the set of desired symbols to analyze.
   */
  class DesiredSymbols {

  public:

    /*!
     *  This map associates each symbol with its symbol information.
     */
    typedef std::map<std::string, SymbolInformation> symbolSet_t;
    symbolSet_t set;

    /*! 
     *  This method constructs a DesiredSymbols instance.
     */
    DesiredSymbols();

    /*! 
     *  This method destructs a DesiredSymbols instance.
     */
    ~DesiredSymbols();

    /*!
     *  This method analyzes each symbols coverage map to determine any
     *  uncovered ranges or branches.
     */
    void computeUncovered( void );

    /*!
     *  This method creates a coverage map for the specified symbol
     *  using the specified size.
     *
     *  @param[in] symbolName specifies the symbol for which to create
     *             a coverage map
     *  @param[in] size specifies the size of the coverage map to create
     */
    void createCoverageMap(
      const std::string& symbolName,
      uint32_t           size
    );

    /*!
     *  This method looks up the symbol information for the specified symbol.
     *
     *  @param[in] symbolName specifies the symbol for which to search
     *
     *  @return Returns a pointer to the symbol's information
     */
    SymbolInformation* find(
      const std::string& symbolName
    );

    /*!
     *  This method determines the source lines that correspond to any
     *  uncovered ranges or branches.
     */
    void findSourceForUncovered( void );

    /*!
     *  This method returns the total number of branches always taken
     *  for all analyzed symbols.
     *
     *  @return Returns the total number of branches always taken
     */
    uint32_t getNumberBranchesAlwaysTaken( void ) const;

    /*!
     *  This method returns the total number of branches found for
     *  all analyzed symbols.
     *
     *  @return Returns the total number of branches found
     */
    uint32_t getNumberBranchesFound( void ) const;

    /*!
     *  This method returns the total number of branches never taken
     *  for all analyzed symbols.
     *
     *  @return Returns the total number of branches never taken
     */
    uint32_t getNumberBranchesNeverTaken( void ) const;

    /*!
     *  This method returns the total number of uncovered ranges
     *  for all analyzed symbols.
     *
     *  @return Returns the total number of uncovered ranges
     */
    uint32_t getNumberUncoveredRanges( void ) const;

    /*!
     *  This method returns an indication of whether or not the specified
     *  symbol is a symbol to analyze.
     *
     *  @return Returns TRUE if the specified symbol is a symbol to analyze
     *   and FALSE otherwise.
     */
    bool isDesired (
      const std::string& symbolName
    ) const;

    /*!
     *  This method creates the set of symbols to analyze from the symbols
     *  listed in the specified file.
     */
    void load(
      const char* const symbolsFile
    );

    /*!
     *  This method merges the coverage information from the source
     *  coverage map into the unified coverage map for the specified symbol.
     *
     *  @param[in] symbolName specifies the symbol associated with the
     *             destination coverage map
     *  @param[in] sourceCoverageMap specifies the source coverage map
     */
    void mergeCoverageMap(
      const std::string&           symbolName,
      const CoverageMapBase* const sourceCoverageMap
    );

    /*!
     *  This method preprocesses each symbol's coverage map to mark nop
     *  and branch information.
     */
    void preprocess( void );

  private:

    /*!
     *  This member variable contains the total number of branches always
     *  taken for all analyzed symbols.
     */
    int branchesAlwaysTaken;

    /*!
     *  This member variable contains the total number of branches found
     *  for all analyzed symbols.
     */
    int branchesFound;

    /*!
     *  This member variable contains the total number of branches never
     *  taken for all analyzed symbols.
     */
    int branchesNeverTaken;

    /*
     *  This member variable contains the total number of uncovered ranges
     *  for all analyzed symbols.
     */
    int uncoveredRanges;

    /*!
     *  This method uses the specified executable file to determine the
     *  source lines for the elements in the specified ranges.
     */
    void determineSourceLines(
      CoverageRanges* const theRanges,
      const std::string&    executableFileName
    );

  };
}

#endif
