/*
 *  $Id$
 */

/*! @file DesiredSymbols.cc
 *  @brief DesiredSymbols Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide the functionality of the DesiredSymbols.
 */

#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "DesiredSymbols.h"
#include "app_common.h"
#include "CoverageMap.h"

namespace Coverage {

  DesiredSymbols::DesiredSymbols()
  {
    branchesAlwaysTaken = 0;
    branchesFound = 0;
    branchesNeverTaken = 0;
    uncoveredRanges = 0;
  }

  DesiredSymbols::~DesiredSymbols()
  {
  }

  void DesiredSymbols::load(
    const char* const symbolsFile
  )
  {
    #define MAX_LINE_LENGTH 512
    char                    buffer[MAX_LINE_LENGTH];
    char*                   cStatus;
    bool                    done = false;
    FILE*                   sFile;
    SymbolInformation*      symInfo;
    int                     line = 1;
    std::string             symbol;

    // Ensure that symbols file name is given.
    if ( !symbolsFile ) {
      fprintf(
        stderr,
        "ERROR: DesiredSymbols::load - no symbols file specified\n"
      );
      exit(-1);
    }

    // Open symbols file.
    sFile = fopen( symbolsFile, "r" );
    if ( !sFile ) {
      fprintf(
        stderr,
        "ERROR: DesiredSymbols::load - unable to open symbols file %s\n",
        symbolsFile
      );
      exit(-1);
    }

    // Process symbols file.
    while ( !done ) {

      symInfo = new SymbolInformation;

      // Skip blank lines between symbols
      do { 
        buffer[0] = '\0';
        cStatus = fgets( buffer, MAX_LINE_LENGTH, sFile );
        if ( cStatus == NULL ) {
          done = true;
        }
        else {
          buffer[ strlen(buffer) - 1] = '\0';
          line++;
        }
      } while ( !done && (buffer[0] == '\0') );

      // Have we already seen this one?
      if ( !done ) {
        if (set.find( buffer ) != set.end()) {
          fprintf(
            stderr,
            "File: %s, Line %d: Duplicate symbol: %s\n",
            symbolsFile,
            line,
            buffer
          );
        }

        // Add this to the set of symbols.
        else {
          set[ buffer ] = *symInfo;
        }
      }
    }
    #undef MAX_LINE_LENGTH
  }

  void DesiredSymbols::computeUncovered( void )
  {
    uint32_t                              a, la, ha;
    uint32_t                              endAddress;
    DesiredSymbols::symbolSet_t::iterator itr;
    CoverageRanges*                       theBranches;
    CoverageMapBase*                      theCoverageMap;
    CoverageRanges*                       theRanges;

    // Look at each symbol.
    for (itr = SymbolsToAnalyze->set.begin();
         itr != SymbolsToAnalyze->set.end();
         itr++) {

      // Create containers for the symbol's uncovered ranges and branches.
      theRanges = new CoverageRanges();
      theBranches = new CoverageRanges();

      // Scan through the symbol's unified coverage map.
      theCoverageMap = itr->second.unifiedCoverageMap;
      endAddress = itr->second.size - 1;
      a = 0;
      while (a <= endAddress) {

        // Find all the unexecuted addresses and add them to the range.
        if (!theCoverageMap->wasExecuted( a )) {
          la = a;
          for (ha=a+1;
               ha<=endAddress && !theCoverageMap->wasExecuted( ha );
               ha++)
            ;
          ha--;

          uncoveredRanges++;
          theRanges->add(
            itr->second.baseAddress + la,
            itr->second.baseAddress + ha,
            CoverageRanges::UNCOVERED_REASON_NOT_EXECUTED
          );
          a = ha + 1;
        }

        else if (theCoverageMap->isBranch( a )) {
          branchesFound++;
          la = a;
          for (ha=a+1;
               ha<=endAddress && !theCoverageMap->isStartOfInstruction( ha );
               ha++)
            ;
          ha--;

          if (theCoverageMap->wasAlwaysTaken( la )) {
            branchesAlwaysTaken++;
            theBranches->add(
              itr->second.baseAddress + la,
              itr->second.baseAddress + ha,
              CoverageRanges::UNCOVERED_REASON_BRANCH_ALWAYS_TAKEN
            );
            if (Verbose)
              fprintf(
                stderr,
                "Branch always taken found in %s (0x%x - 0x%x)\n",
                (itr->first).c_str(),
                itr->second.baseAddress + la,
                itr->second.baseAddress + ha
              );
          }
          else if (theCoverageMap->wasNeverTaken( la )) {
            branchesNeverTaken++;
            theBranches->add(
              itr->second.baseAddress + la,
              itr->second.baseAddress + ha,
              CoverageRanges::UNCOVERED_REASON_BRANCH_NEVER_TAKEN
            );
            if (Verbose)
              fprintf(
                stderr,
                "Branch never taken found in %s (0x%x - 0x%x)\n",
                (itr->first).c_str(),
                itr->second.baseAddress + la,
                itr->second.baseAddress + ha
              );
          }
          a = ha + 1;
        }
        else
          a++;
      }

      // Save the uncovered ranges and branches containers.
      itr->second.uncoveredRanges = theRanges;
      itr->second.uncoveredBranches = theBranches;
    }
  }

  void DesiredSymbols::createCoverageMap(
    const std::string& symbolName,
    uint32_t           size
  )
  {
    CoverageMapBase*      aCoverageMap;
    uint32_t              highAddress;
    symbolSet_t::iterator itr;

    // Ensure that the symbol is a desired symbol.
    itr = set.find( symbolName );

    if (itr == set.end()) {

      fprintf(
        stderr,
        "ERROR: DesiredSymbols::createCoverageMap - Unable to create "
        "unified coverage map for %s because it is NOT a desired symbol\n",
        symbolName.c_str()
      );
      exit( -1 );
    }

    // If we have already created a coverage map, ...
    if (itr->second.unifiedCoverageMap) {

      // ensure that the specified size matches the existing size.
      if (itr->second.size != size) {

        fprintf(
          stderr,
          "ERROR: DesiredSymbols::createCoverageMap - Attempt to create "
          "unified coverage maps for %s with different sizes\n",
          symbolName.c_str()
        );
        exit( -1 );
      }
    }

    // If we don't already have a coverage map, create one.
    else {

      highAddress = size - 1;

      aCoverageMap = new CoverageMap( 0, highAddress );
      if (!aCoverageMap) {

        fprintf(
          stderr,
          "ERROR: DesiredSymbols::createCoverageMap - Unable to allocate "
          "coverage map for %s\n",
          symbolName.c_str()
        );
        exit( -1 );
      }

      if ( Verbose )
        fprintf(
          stderr,
          "Created unified coverage map for %s (0x%x - 0x%x)\n",
          symbolName.c_str(), 0, highAddress
        );
      itr->second.unifiedCoverageMap = aCoverageMap;
      itr->second.size = size;
    }
  }

  void DesiredSymbols::determineSourceLines(
    CoverageRanges* const theRanges,
    const std::string&    executableFileName
  )
  {
    char*                              base;
    char                               buffer[512];
    char*                              cStatus;
    char                               command[512];
    CoverageRanges::ranges_t::iterator ritr;
    char                               rpath[PATH_MAX];
    FILE*                              tmpfile;

    // Open a temporary file for the uncovered ranges.
    tmpfile = fopen( "ranges1.tmp", "w" );
    if ( !tmpfile ) {
      fprintf(
        stderr,
        "ERROR: DesiredSymbols::determineSourceLines - "
        "unable to open %s\n",
        "ranges1.tmp"
      );
      exit(-1);
    }

    // Write the range addresses to the temporary file.
    for (ritr =  theRanges->set.begin();
         ritr != theRanges->set.end();
         ritr++ ) {
      fprintf(
        tmpfile,
        "0x%08x\n0x%08x\n",
        ritr->lowAddress,
        ritr->highAddress
      );
    }

    fclose( tmpfile );

    // Invoke addr2line to generate the source lines for each address.
    sprintf(
      command,
      "%s -e %s <%s | dos2unix >%s",
      Tools->getAddr2line(),
      executableFileName.c_str(),
      "ranges1.tmp",
      "ranges2.tmp"
    );
    if (system( command )) {
      fprintf(
        stderr,
        "ERROR: DesiredSymbols::determineSourceLines - "
        "command (%s) failed\n",
        command
      );
      exit( -1 );
    }

    // Open the addr2line output file.
    tmpfile = fopen( "ranges2.tmp", "r" );
    if ( !tmpfile ) {
      fprintf(
        stderr,
        "ERROR: DesiredSymbols::determineSourceLines - "
        "unable to open %s\n",
        "ranges2.tmp"
      );
      exit(-1);
    }

    // Process the addr2line output.
    for (ritr =  theRanges->set.begin();
         ritr != theRanges->set.end();
         ritr++ ) {

      cStatus = fgets( buffer, 512, tmpfile );
      if ( cStatus == NULL ) {
        fprintf(
          stderr,
          "ERROR: DesiredSymbols::determineSourceLines - "
          "Out of sync in addr2line output\n"
        );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';

      // Use only the base filename without directory path.
      realpath( buffer, rpath );
      base = basename( rpath );

      ritr->lowSourceLine = std::string( base );

      cStatus = fgets( buffer, 512, tmpfile );
      if ( cStatus == NULL ) {
        fprintf(
          stderr,
          "ERROR: DesiredSymbols::determineSourceLines - "
          "Out of sync in addr2line output\n"
        );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';

      // Use only the base filename without directory path.
      realpath( buffer, rpath );
      base = basename( rpath );

      ritr->highSourceLine = std::string( base );
    }

    fclose( tmpfile );
  }

  SymbolInformation* DesiredSymbols::find(
    const std::string& symbolName
  )
  {
    if (set.find( symbolName ) == set.end())
      return NULL;
    else
      return &set[ symbolName ];
  }

  void DesiredSymbols::findSourceForUncovered( void )
  {
    DesiredSymbols::symbolSet_t::iterator ditr;
    CoverageRanges*                       theBranches;
    CoverageRanges*                       theRanges;

    // Process uncovered ranges and/or branches for each symbol.
    for (ditr = SymbolsToAnalyze->set.begin();
         ditr != SymbolsToAnalyze->set.end();
         ditr++) {

      // First the unexecuted ranges, ...
      theRanges = ditr->second.uncoveredRanges;

      if (!theRanges->set.empty()) {
        if (Verbose)
          fprintf(
            stderr,
            "Looking up source lines for uncovered ranges in %s\n",
            (ditr->first).c_str()
          );
        determineSourceLines(
          theRanges,
          ditr->second.sourceFile
        );
      }

      // then the uncovered branches.
      theBranches = ditr->second.uncoveredBranches;

      if (!theBranches->set.empty()) {
        if (Verbose)
          fprintf(
            stderr,
            "Looking up source lines for uncovered branches in %s\n",
            (ditr->first).c_str()
          );
        determineSourceLines(
          theBranches,
          ditr->second.sourceFile
        );
      }
    }
  }

  uint32_t DesiredSymbols::getNumberBranchesAlwaysTaken( void ) const {
    return branchesAlwaysTaken;
  };

  uint32_t DesiredSymbols::getNumberBranchesFound( void ) const {
    return branchesFound;
  };

  uint32_t DesiredSymbols::getNumberBranchesNeverTaken( void ) const {
    return branchesNeverTaken;
  };

  uint32_t DesiredSymbols::getNumberUncoveredRanges( void ) const {
    return uncoveredRanges;
  };

  bool DesiredSymbols::isDesired (
    const std::string& symbolName
  ) const
  {
    if (set.find( symbolName ) == set.end()) {
      #if 0
        fprintf( stderr, 
          "Warning: Unable to find symbol %s\n", 
          symbolName.c_str() 
        );
      #endif
      return false;
    }
    return true;
  }

  void DesiredSymbols::mergeCoverageMap(
    const std::string&           symbolName,
    const CoverageMapBase* const sourceCoverageMap
  )
  {
    uint32_t              dAddress;
    CoverageMapBase*      destinationCoverageMap;
    uint32_t              dMapSize;
    symbolSet_t::iterator itr;
    uint32_t              sAddress;
    uint32_t              sBaseAddress;
    uint32_t              sMapSize;

    // Ensure that the symbol is a desired symbol.
    itr = set.find( symbolName );

    if (itr == set.end()) {

      fprintf(
        stderr,
        "ERROR: DesiredSymbols::mergeCoverageMap - Unable to merge "
        "coverage map for %s because it is NOT a desired symbol\n",
        symbolName.c_str()
      );
      exit( -1 );
    }

    // Ensure that the source and destination coverage maps
    // are the same size.
    dMapSize = itr->second.size;
    sBaseAddress = sourceCoverageMap->getLowAddress();
    sMapSize = sourceCoverageMap->getHighAddress() - sBaseAddress + 1;
    if (dMapSize != sMapSize) {

      fprintf(
        stderr,
        "ERROR: DesiredSymbols::mergeCoverageMap - Unable to merge "
        "coverage map for %s because the sizes are different\n",
        symbolName.c_str()
      );
      exit( -1 );
    }

    // Merge the data for each address.
    destinationCoverageMap = itr->second.unifiedCoverageMap;

    for (dAddress = 0; dAddress < dMapSize; dAddress++) {

      sAddress = dAddress + sBaseAddress;

      // Merge start of instruction indication.
      if (sourceCoverageMap->isStartOfInstruction( sAddress ))
        destinationCoverageMap->setIsStartOfInstruction( dAddress );

      // Merge the execution data.
      if (sourceCoverageMap->wasExecuted( sAddress))
        destinationCoverageMap->setWasExecuted( dAddress );

      // Merge the branch data.
      if (sourceCoverageMap->isBranch( sAddress )) {

        destinationCoverageMap->setIsBranch( dAddress );

        if (sourceCoverageMap->wasTaken( sAddress))
          destinationCoverageMap->setWasTaken( dAddress );

        if (sourceCoverageMap->wasNotTaken( sAddress))
          destinationCoverageMap->setWasNotTaken( dAddress );
      }
    }
  }

}
