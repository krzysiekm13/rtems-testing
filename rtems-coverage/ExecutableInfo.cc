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
    // Create the symbol table.
    theSymbolTable = new SymbolTable();
  }

  CoverageMapBase* ExecutableInfo::createCoverageMap (
    const std::string& symbolName,
    uint32_t           lowAddress,
    uint32_t           highAddress
  )
  {
    CoverageMapBase* theMap = NULL;

    theMap = new CoverageMap( lowAddress, highAddress );

    if (!theMap)
      fprintf(
        stderr, "Unable to create coverage map for %s\n",
        symbolName.c_str()
      );

    else
      coverageMaps[ symbolName ] = theMap;

    return theMap;
  }

  void ExecutableInfo::mergeCoverage( void ) {
    ExecutableInfo::coverageMaps_t::iterator  itr;

    for (itr = coverageMaps.begin(); itr != coverageMaps.end(); itr++) {
      SymbolsToAnalyze->mergeCoverageMap( (*itr).first, (*itr).second );
    }
  }

}
