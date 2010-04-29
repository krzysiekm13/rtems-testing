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

#include "ObjdumpProcessor.h"
#include "app_common.h"
#include "ExecutableInfo.h"
#include "TargetFactory.h"

namespace Coverage {

  ObjdumpProcessor::ObjdumpProcessor()
  {
    target_m = Target::TargetFactory(Tools->getTarget());
  }

  ObjdumpProcessor::~ObjdumpProcessor()
  {
  }

  bool ObjdumpProcessor::IsBranch(
    const char *instruction 
  )
  { 
    if (!target_m) {
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      assert(0);
      return false;
    }

    return target_m->isBranch( instruction );
  }

  bool ObjdumpProcessor::isBranchLine(
    const char* const line
  )
  {
    if (!target_m) {
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      assert(0);
      return false;
    }

    return  target_m->isBranchLine( line );
  }

  bool ObjdumpProcessor::isNop(
    const char* const line,
    int&              size
  )
  {

    if (!target_m){
      fprintf( stderr, "ERROR!!! unknown architecture!!!\n");
      fprintf( stderr, "HOW LARGE IS NOP ON THIS ARCHITECTURE? -- fix me\n" );
      assert(0);
      return false;
    }

    return target_m->isNopLine( line, size );
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
        "%s -da --source %s | sed -e \'s/ *$//\' >%s",
        Tools->getObjdump(),
         exeFileName.c_str(),
        dumpFile
      );

      status = system( buffer );
      if (status) {
        fprintf(
          stderr,
          "ERROR: ObjdumpProcessor::load - command (%s) failed with %d\n",
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
        "ERROR: ObjdumpProcessor::load - unable to open %s\n",
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
    uint32_t           baseAddress;
    char               buffer[ 512 ];
    char*              cStatus;
    uint32_t           endAddress = 0xffffffff;
    uint32_t           instructionAddress;
    int                items;
    objdumpLine_t      lineInfo;
    FILE*              objdumpFile;
    bool               processSymbol = false;
    bool               saveInstructionDump = false;
    char               symbol[ 100 ];
    SymbolInformation* symbolInfo = NULL;
    char               terminator;


    objdumpFile = getFile( executableInformation->getFileName() );

    // Process all lines from the objdump file.
    while ( 1 ) {

      // Get the line.
      cStatus = fgets( buffer, 512, objdumpFile );
      if (cStatus == NULL) {
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
      // address and symbol.
      items = sscanf(
        buffer,
        "%x <%[^>]>%c",
        &baseAddress, symbol, &terminator
      );

      // If all items found ...
      if ((items == 3) && (terminator == ':')) {

        // we are at the beginning of a symbol's objdump and
        // must end any processing of the previous symbol.
        processSymbol = false;
        saveInstructionDump = false;

        // See if the symbol is one that we care about.
        symbolInfo = SymbolsToAnalyze->find( symbol );

        // If it is, ...
        if (symbolInfo) {

          // indicate that we are processing a symbols objdump and
          // compute the ending address for termination.
          processSymbol = true;
          endAddress = baseAddress +
           executableInformation->getSymbolTable()->getLength( symbol ) - 1;

          // If there are NOT already instructions available, indicate that they
          // are to be saved.
          if (symbolInfo->instructions.empty()) {
            saveInstructionDump = true;
            symbolInfo->sourceFile = executableInformation->getFileName();
            symbolInfo->baseAddress = baseAddress;
          }
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

          // and we are NOT beyond the end of the symbol's objdump,
          if (instructionAddress <= endAddress) {

            // update the line's information and ...
            lineInfo.address       = instructionAddress;
            lineInfo.isInstruction = true;
            lineInfo.isNop         = isNop( buffer, lineInfo.nopSize );
            lineInfo.isBranch      = isBranchLine( buffer );

            // mark the address as the beginning of an instruction.
            executableInformation->markStartOfInstruction( instructionAddress );
          }

          // If we are beyond the end of the symbol's objdump,
          // it's time to end processing of this symbol.
          else {
            processSymbol = false;
            saveInstructionDump = false;
          }
        }
      }

      // If we are processing a symbol, ...
      if (processSymbol && saveInstructionDump) {

        // add line to the current symbol's information and ...
        symbolInfo->instructions.push_back( lineInfo );

      }
    }
  }

}
