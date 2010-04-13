/*
 *  $Id$
 */

/*! @file SymbolTable.cc
 *  @brief SymbolTable Implementation
 *
 *  This file contains ...
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SymbolTable.h"
#include "app_common.h"

namespace Coverage {

  SymbolTable::SymbolTable()
  {
  }

  SymbolTable::~SymbolTable()
  {
  }

  SymbolTable::symbolInfo* SymbolTable::getInfo(
    const std::string& symbol
  )
  {
    info_t::iterator it = info.find( symbol );

    if (it == info.end())
      return NULL;
    else
      return (&(it->second));
  }

  uint32_t SymbolTable::getLength(
    const std::string& symbol
  )
  {
    info_t::iterator it = info.find( symbol );

    if (it == info.end())
      return 0;
    else
      return ((*it).second.length);
  }

  std::string SymbolTable::getSymbol(
    uint32_t address
  )
  {
    contents_t::iterator it = contents.end();

    it--;
    if (address > ((*it).first)) {
      return "";
    }

    it = contents.lower_bound( address );
    if (((*it).second).min <= address ) {
      return ((*it).second).symbol;
    } else {
      return "";
    }
  }

  bool SymbolTable::load(
    const std::string& executableName
  )
  {
    uint32_t    address = 0;
    char        buffer[512];
    char       *cStatus;
    uint32_t    end = 0;
    int         items;
    uint32_t    length = 0;
    FILE       *nmFile = NULL;
    uint32_t    start = 0;
    char        symbol[ 100 ];
    symbolInfo  symbolData;

    // Generate the nm output
    sprintf( buffer, "%s -n -f sysv %s | sed -e \'s/ *$//\' >nm.tmp",
      Tools->getNm(), executableName.c_str() );

    if ( system( buffer ) ) {
      fprintf(
        stderr,
        "ERROR: SymbolTable::load - command (%s) failed\n",
        buffer
      );
      exit( -1 );
    }

    // Read the file and process each desired symbol
    nmFile = fopen( "nm.tmp", "r" );
    if ( !nmFile ) {
      fprintf(
        stderr,
        "ERROR: SymbolTable::load - unable to open %s\n",
        "nm.tmp"
      );
      exit(-1);
    }

    // Process all lines from the nm file.
    while ( 1 ) {

      // Get the line.
      cStatus = fgets( buffer, 512, nmFile );
      if ( cStatus == NULL ) {
        break;
      }
      buffer[ strlen(buffer) - 1] = '\0';

      // Extract the symbol, start address and length.
      items = sscanf(
        buffer,
        "%[^| ] |%x|%*[^|]| FUNC|%x",
        symbol, &start, &length
      );

      // If all items found ...
      if (items == 3) {

        // and it is a desired symbol, ...
        if (SymbolsToAnalyze->isDesired( symbol )) {

          // add the symbol information to the symbol table.
          end = start + length - 1;
          symbol_entry_t entry = { start, end, symbol };
          contents[end] = entry;

          symbolData.startingAddress = start;
          symbolData.length = length;
          info[ symbol ] = symbolData;

          // Also create a coverage map for the results of the analysis.
          SymbolsToAnalyze->createCoverageMap( symbol, length );
        }
      }
    }

    fclose( nmFile );

    // Remove temporary file
    (void) system( "rm -f nm.tmp" );
    return true;
  }

}
