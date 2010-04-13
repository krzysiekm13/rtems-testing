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

#include "ObjdumpProcessor.h"
#include "app_common.h"
#include "ExecutableInfo.h"

namespace Coverage {

  ObjdumpProcessor::ObjdumpProcessor()
  {
  }

  ObjdumpProcessor::~ObjdumpProcessor()
  {
  }

  bool ObjdumpProcessor::isNop(
    const char* const line,
    int&              size
  )
  {
    bool        isNop = false;
    const char *target = Tools->getTarget();

    size = 0;

    // common patterns
    if (!strcmp( &line[strlen(line)-3], "nop"))
      isNop = true;
    
    // now check target specific patterns and return proper size if "nop"

    // Check ARM nops
    if (!strncmp( target, "arm", 3 )) {
      if (isNop) {
        size = 4; 
        return true;
      }
    
      // On ARM, there are literal tables at the end of methods.
      // We need to avoid them.
      if (!strncmp( &line[strlen(line)-10], ".byte", 5)) {
        size = 1;
        return true;
      }
      if (!strncmp( &line[strlen(line)-13], ".short", 6)) {
        size = 2;
        return true;
      }
      if (!strncmp( &line[strlen(line)-16], ".word", 5)) {
        size = 4;
        return true;
      }

      return false;
    }

    // Check i386 nops
    if (!strncmp( target, "i386", 4 )) {
      if (isNop) {
        size = 1; 
        return true;
      }
      // i386 has some two and three byte nops
      if (!strncmp( &line[strlen(line)-14], "xchg   %ax,%ax", 14)) {
        size = 2;
        return true;
      }
      if (!strncmp( &line[strlen(line)-16], "xor    %eax,%eax", 16)) {
        size = 2;
        return true;
      }
      if (!strncmp( &line[strlen(line)-16], "xor    %ebx,%ebx", 16)) {
        size = 2;
        return true;
      }
      if (!strncmp( &line[strlen(line)-16], "xor    %esi,%esi", 16)) {
        size = 2;
        return true;
      }
      if (!strncmp( &line[strlen(line)-21], "lea    0x0(%esi),%esi", 21)) {
        size = 3;
        return true;
      }

      return false;
    }

    // Check M68K/Coldfire nops
    if (!strncmp( target, "m68k", 4 )) {
      if (isNop) {
        size = 2; 
        return true;
      }

      #define GNU_LD_FILLS_ALIGNMENT_WITH_RTS
      #if defined(GNU_LD_FILLS_ALIGNMENT_WITH_RTS)
        // Until binutils 2.20, binutils would fill with rts not nop
        if (!strcmp( &line[strlen(line)-3], "rts")) {
          size = 4; 
          return true;
        } 
      #endif

      return false;
    }

    // Check powerpc nops
    if (!strncmp( target, "powerpc", 7 )) {
      if (isNop) {
        size = 4; 
        return true;
      }

      return false;
    }

    // Check powerpc nops
    if (!strncmp( target, "lm32", 4 )) {
      if (isNop) {
        size = 4; 
        return true;
      }

      return false;
    }

    // Check SPARC nops
    if (!strncmp( target, "sparc", 5 )) {
      if (isNop) {
        size = 4; 
        return true;
      }

      if (!strcmp( &line[strlen(line)-7], "unknown")) {
        size = 4; 
        return true;
      } 

      return false;
    }

    fprintf( stderr, "ERROR!!! %s is not a known architecture!!!\n", target );
    fprintf( stderr, "HOW LARGE IS NOP ON THIS ARCHITECTURE? -- fix me\n" );
    assert(0);
    // ASSUME: ARM dump uses nop instruction. Really "mov r0,r0"
    return false;
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
    int                status;
    char               dumpFile[128];

    sprintf(
      dumpFile,
      "%s.dmp",
      executableInformation->getFileName().c_str()
    );
      
    // Generate the objdump.
    sprintf(
      buffer,
      "%s -da --source %s | sed -e \'s/ *$//\' >%s",
      Tools->getObjdump(),
      (executableInformation->getFileName()).c_str(),
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
