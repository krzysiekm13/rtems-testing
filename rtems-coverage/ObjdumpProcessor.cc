/*
 *  $Id$
 */

/*! @file ObjdumpProcessor.cc
 *  @brief ObjdumpProcessor Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  the reading of an objdump output file and adding nops to a
 *  coverage map. 
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "app_common.h"
#include "ObjdumpProcessor.h"
#include "CoverageMap.h"
#include "ExecutableInfo.h"
#include "SymbolTable.h"
#include "TargetFactory.h"

namespace Coverage {

  void finalizeSymbol(
    ExecutableInfo* const            executableInfo,
    std::string&                     symbolName,
    uint32_t                         lowAddress,
    uint32_t                         highAddress,
    ObjdumpProcessor::objdumpLines_t instructions,
    SymbolInformation*               symbolInfo
  ) {

    CoverageMapBase*                           aCoverageMap = NULL;
    ObjdumpProcessor::objdumpLines_t::iterator itr;
    SymbolTable*                               theSymbolTable;

    // If there are NOT already saved instructions, save them.
    if (symbolInfo->instructions.empty()) {
      symbolInfo->sourceFile = executableInfo->getFileName();
      symbolInfo->baseAddress = lowAddress;
      symbolInfo->instructions = instructions;
    }

    // Add the symbol to this executable's symbol table.
    theSymbolTable = executableInfo->getSymbolTable();
    theSymbolTable->addSymbol(
      symbolName, lowAddress, highAddress - lowAddress + 1
    );

    // Create a coverage map for the symbol.
    aCoverageMap = executableInfo->createCoverageMap(
      symbolName, lowAddress, highAddress
    );

    if (aCoverageMap) {

      // Mark the start of each instruction in the coverage map.
      for (itr = instructions.begin();
           itr != instructions.end();
           itr++ ) {

        aCoverageMap->setIsStartOfInstruction( itr->address );
      }

      // Create a unified coverage map for the symbol.
      SymbolsToAnalyze->createCoverageMap(
        symbolName, highAddress - lowAddress + 1
      );
    }
  }

  ObjdumpProcessor::ObjdumpProcessor()
  {
  }

  ObjdumpProcessor::~ObjdumpProcessor()
  {
  }

  bool ObjdumpProcessor::IsBranch(
    const char *instruction 
  )
  { 
    if ( !TargetInfo ) {
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      assert(0);
      return false;
    }

    return TargetInfo->isBranch( instruction );
  }

  bool ObjdumpProcessor::isBranchLine(
    const char* const line
  )
  {
    if ( !TargetInfo ) {
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      assert(0);
      return false;
    }

    return  TargetInfo->isBranchLine( line );
  }

  bool ObjdumpProcessor::isNop(
    const char* const line,
    int&              size
  )
  {

    if ( !TargetInfo ){
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      fprintf( stderr, "HOW LARGE IS NOP ON THIS ARCHITECTURE? -- fix me\n" );
      assert(0);
      return false;
    }

    return TargetInfo->isNopLine( line, size );
  }

  FILE* ObjdumpProcessor::getFile( 
    std::string exeFileName 
  ) 
  {
    char               dumpFile[128];
    FILE*              objdumpFile;
    char               buffer[ 512 ];
    int                status;

    sprintf(dumpFile,"%s.dmp", exeFileName.c_str() );
      
    // Generate the objdump.
    if ( FileIsNewer( exeFileName.c_str(), dumpFile )) {
      sprintf(
        buffer,
        "%s -da --section=.text --source %s | sed -e \'s/ *$//\' >%s",
        Tools->getObjdump(),
         exeFileName.c_str(),
        dumpFile
      );

      status = system( buffer );
      if (status) {
        fprintf(
          stderr,
          "ERROR: ObjdumpProcessor::getFile - command (%s) failed with %d\n",
          buffer,
          status
        );
        exit( -1 );
      }
    } 

    // Open the objdump file.
    objdumpFile = fopen( dumpFile, "r" );
    if (!objdumpFile) {
      fprintf(
        stderr,
        "ERROR: ObjdumpProcessor::getFile - unable to open %s\n",
        dumpFile
      );
      exit(-1);
    }

    return objdumpFile;
  }

  uint32_t ObjdumpProcessor::getAddressAfter( uint32_t address )
  {
    objdumpFile_t::iterator itr;

    itr = find ( objdumpList.begin(), objdumpList.end(), address );
    if (itr == objdumpList.end()) {
      return 0;
    }
    
    itr++;
    if (itr == objdumpList.end()) {
      return 0;
    }

    return (*itr);

  }

  void ObjdumpProcessor::loadAddressTable (std::string executableFileName )
  {
    char               buffer[ 512 ];
    char*              cStatus;
    uint32_t           instructionAddress;
    int                items;
    FILE*              objdumpFile;
    char               terminator;

    objdumpFile = getFile( executableFileName );

    // Process all lines from the objdump file.
    while ( 1 ) {

      // Get the line.
      cStatus = fgets( buffer, 512, objdumpFile );
      if (cStatus == NULL) {
        break;
      }
      buffer[ strlen(buffer) - 1] = '\0';

      // See if it is the dump of an instruction.
      items = sscanf(
        buffer,
        "%x%c",
        &instructionAddress, &terminator
      );

      // If it looks like an instruction ...
      if ((items == 2) && (terminator == ':')){
        objdumpList.push_back(instructionAddress);
      }
    }
  }

  void ObjdumpProcessor::load(
    ExecutableInfo* const executableInformation
  )
  {
    uint32_t           address;
    uint32_t           baseAddress = 0;
    char               buffer[ 512 ];
    char*              cStatus;
    std::string        currentSymbol = "";
    uint32_t           instructionAddress;
    int                items;
    objdumpLine_t      lineInfo;
    FILE*              objdumpFile;
    bool               processSymbol = false;
    char               symbol[ 100 ];
    SymbolInformation* symbolInformation = NULL;
    char               terminator;
    objdumpLines_t     theInstructions;

    // Obtain the objdump file.
    objdumpFile = getFile( executableInformation->getFileName() );

    // Process all lines from the objdump file.
    while ( 1 ) {

      // Get the line.
      cStatus = fgets( buffer, 512, objdumpFile );
      if (cStatus == NULL) {

        // If we are currently processing a symbol, finalize it.
        if ((processSymbol) && (symbolInformation)) {
          finalizeSymbol(
            executableInformation,
            currentSymbol,
            baseAddress,
            address,  // XXX fix to determine corrent end address
            theInstructions,
            symbolInformation
          );
          fprintf(
            stderr,
            "WARNING: ObjdumpProcessor::load - analysis of symbol %s \n"
            "         may be incorrect.  It was the last symbol in %s\n"
            "         and the length of its last instruction is assumed to be one.\n",
            currentSymbol.c_str(),
            executableInformation->getFileName().c_str()
          );
        }
        break;
      }

      buffer[ strlen(buffer) - 1] = '\0';

      lineInfo.line          = buffer;
      lineInfo.address       = 0xffffffff;
      lineInfo.isInstruction = false;
      lineInfo.isNop         = false;
      lineInfo.nopSize       = 0;
      lineInfo.isBranch      = false;

      // Look for the start of a symbol's objdump and extract
      // address and symbol (i.e. address <symbolname>:).
      items = sscanf(
        buffer,
        "%x <%[^>]>%c",
        &address, symbol, &terminator
      );

      // If all items found, we are at the beginning of a symbol's objdump.
      if ((items == 3) && (terminator == ':')) {

        // If we are currently processing a symbol, finalize it.
        if ((processSymbol) && (symbolInformation)) {
          finalizeSymbol(
            executableInformation,
            currentSymbol,
            baseAddress,
            address - 1,
            theInstructions,
            symbolInformation
          );
        }

        // Start processing of a new symbol.
        baseAddress = 0;
        currentSymbol = "";
        processSymbol = false;
        theInstructions.clear();

        // See if the new symbol is one that we care about.
        symbolInformation = SymbolsToAnalyze->find( symbol );

        if (symbolInformation) {
          baseAddress = address;
          currentSymbol = symbol;
          processSymbol = true;
          theInstructions.push_back( lineInfo );
        }
      }

      else if (processSymbol) {

        // See if it is the dump of an instruction.
        items = sscanf(
          buffer,
          "%x%c",
          &instructionAddress, &terminator
        );

        // If it looks like an instruction ...
        if ((items == 2) && (terminator == ':')) {

          // update the line's information, save it and ...
          lineInfo.address       = instructionAddress;
          lineInfo.isInstruction = true;
          lineInfo.isNop         = isNop( buffer, lineInfo.nopSize );
          lineInfo.isBranch      = isBranchLine( buffer );
        }

        // Always save the line.
        theInstructions.push_back( lineInfo );
      }
    }
  }
}
