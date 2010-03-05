/*
 * $Id$
 */

/*! @file ExecutableInfo.cc
 *  @brief ExecutableInfo Implementation
 *
 *  This file contains the implementation of the functionality
 *  of the ExecutableInfo class.
 */

#include <stdio.h>

#include "ExecutableInfo.h"
#include "app_common.h"
#include "CoverageMap.h"
#include "DesiredSymbols.h"
#include "SymbolTable.h"

namespace Coverage {

  ExecutableInfo::ExecutableInfo( const char* const executableName )
  {
    fileName = executableName;
    theSymbolTable = NULL;
  }

  ExecutableInfo::~ExecutableInfo()
  {
    if (theSymbolTable)
      delete theSymbolTable;
  }

  void ExecutableInfo::dumpCoverageMaps( void ) {
    ExecutableInfo::coverageMaps_t::iterator  itr;

    for (itr = coverageMaps.begin(); itr != coverageMaps.end(); itr++) {
      fprintf( stderr, "Coverage Map for %s\n", ((*itr).first).c_str() );;
      ((*itr).second)->dump();
    }
  }

  CoverageMapBase* ExecutableInfo::getCoverageMap ( uint32_t address )
  {
    CoverageMapBase*         aCoverageMap = NULL;
    coverageMaps_t::iterator it;
    std::string              itsSymbol;

    // Obtain the coverage map containing the specified address.
    itsSymbol = theSymbolTable->getSymbol( address );
    if (itsSymbol != "") {
      it = coverageMaps.find( itsSymbol );
      aCoverageMap = (*it).second;
    }

    return aCoverageMap;
  }

  std::string ExecutableInfo::getFileName ( void ) const
  {
    return fileName;
  }

  SymbolTable* ExecutableInfo::getSymbolTable ( void ) const
  {
    return theSymbolTable;
  }

  void ExecutableInfo::initialize( void )
  {
    CoverageMapBase                       *aCoverageMap;
    uint32_t                               highAddress;
    SymbolTable::symbolInfo*               info;
    DesiredSymbols::symbolSet_t::iterator  itr;
    uint32_t                               lowAddress;
    std::string                            symbol;

    // Create and initialize symbol table.
    if ( Verbose )
      fprintf( stderr, "Reading symbol table of %s\n", fileName.c_str() );
    theSymbolTable = new SymbolTable();
    theSymbolTable->load( fileName );

    // Create a coverage map for each symbol to analyze.
    for (itr = SymbolsToAnalyze->set.begin();
         itr != SymbolsToAnalyze->set.end();
         itr++) {

      symbol = (*itr).first;
      info = NULL;
      info = theSymbolTable->getInfo( symbol );
      if (!info) {
        if (Verbose)
          fprintf(
            stderr,
            "Unable to find symbol information for %s\n",
            symbol.c_str()
          );
        continue;
      }

      lowAddress = info->startingAddress;
      highAddress = lowAddress + info->length - 1;

      aCoverageMap = NULL;
      aCoverageMap = new CoverageMap( lowAddress, highAddress );
      if (!aCoverageMap) {
        fprintf(
          stderr, "Unable to create coverage map for %s\n", symbol.c_str()
        );
        continue;
      }

      coverageMaps[ symbol ] = aCoverageMap;
    }
  }

  void ExecutableInfo::markBranchNotTaken(
    uint32_t address
  )
  {
    CoverageMapBase* aCoverageMap = NULL;
    uint32_t         beginning;

    // Obtain the coverage map containing the specified address.
    aCoverageMap = getCoverageMap( address );

    // Ensure that coverage map exists.
    if (aCoverageMap) {

      // Determine the beginning address of the instruction that
      // contains the specified address.  We only want to mark the
      // beginning address of the instruction.
      if (aCoverageMap->getBeginningOfInstruction( address, &beginning )) {
        aCoverageMap->setIsBranch( beginning );
        aCoverageMap->setWasNotTaken( beginning );
        if (Verbose)
          fprintf(
            stderr, "Marking branch not taken at 0x%x\n", beginning
          );
      }
    }
  }

  void ExecutableInfo::markBranchTaken(
    uint32_t address
  )
  {
    CoverageMapBase* aCoverageMap = NULL;
    uint32_t         beginning;

    // Obtain the coverage map containing the specified address.
    aCoverageMap = getCoverageMap( address );

    // Ensure that coverage map exists.
    if (aCoverageMap) {

      // Determine the beginning address of the instruction that
      // contains the specified address.  We only want to mark the
      // beginning address of the instruction.
      if (aCoverageMap->getBeginningOfInstruction( address, &beginning )) {
        aCoverageMap->setIsBranch( beginning );
        aCoverageMap->setWasTaken( beginning );
        if (Verbose)
          fprintf(
            stderr, "Marking branch taken at 0x%x\n", beginning
          );
      }
    }
  }

  void ExecutableInfo::markStartOfInstruction(
    uint32_t address 
  )
  {
    CoverageMapBase* aCoverageMap = NULL;

    // Obtain the coverage map containing the specified address.
    aCoverageMap = getCoverageMap( address );

    // Ensure that coverage map exists.
    if (aCoverageMap)
      aCoverageMap->setIsStartOfInstruction( address );
  }

  void ExecutableInfo::markWasExecuted(
    uint32_t address
  )
  {
    CoverageMapBase* aCoverageMap = NULL;

    // Obtain the coverage map containing the specified address.
    aCoverageMap = getCoverageMap( address );

    // Ensure that coverage map exists.
    if (aCoverageMap)
      aCoverageMap->setWasExecuted( address );
  }

  void ExecutableInfo::mergeCoverage( void ) {
    ExecutableInfo::coverageMaps_t::iterator  itr;

    for (itr = coverageMaps.begin(); itr != coverageMaps.end(); itr++) {
      SymbolsToAnalyze->mergeCoverageMap( (*itr).first, (*itr).second );
    }
  }

}
