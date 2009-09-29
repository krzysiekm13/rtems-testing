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

#include "ObjdumpProcessor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "app_common.h"
#include <assert.h>

namespace Coverage {

  class ObjdumpLine {
  
  public:
     ObjdumpLine()
     {
       isInstruction = false;
       isNop         = false;
       nopSize       = 0;
       address       = 0xffffffff;
     } 

     ~ObjdumpLine()
     {
     } 

     std::string line;
     bool        isInstruction;
     bool        isNop;
     int         nopSize;
     uint32_t    address;
  };

  /*
   * ObjdumpProcessor Class
   */
  ObjdumpProcessor::ObjdumpProcessor()
  {
  }

  ObjdumpProcessor::~ObjdumpProcessor()
  {
  }

  bool ObjdumpProcessor::isInstruction(
    const char *line
  )
  {
    int i; 
    bool allBlank = true;
    
    for ( i=0 ; i<8 ; i++ ) {
      if ( isxdigit( line[i] ) ) {
        allBlank = false;
        continue;
      }
      if ( isspace( line[i] ) ) {
        continue;
      }
      return false;
    }

    if ( allBlank )
      return false;

    if ( line[8] != ':' )
      return false;

    return true;
  }

  bool ObjdumpProcessor::isNop(
    const char *line,
    int        &size
  )
  {
    bool        isNop = false;
    const char *target = Tools->getTarget();

    if ( !isInstruction(line) ) {
      size = 0;
      return false;
    }

    size = 0;

    // common patterns
    if ( !strcmp( &line[strlen(line)-3], "nop") )
      isNop = true;

    
    // now check target specific patterns and return proper size if "nop"

    // Check ARM nops
    if ( !strncmp( target, "arm", 3 ) ) {
      if ( isNop ) {
        size = 4; 
        return true;
      }
    
      // On ARM, there are literal tables at the end of methods.
      // We need to avoid them.
      if ( !strncmp( &line[strlen(line)-10], ".byte", 5) ) {
        size = 1;
        return true;
      }
      if ( !strncmp( &line[strlen(line)-13], ".short", 6) ) {
        size = 2;
        return true;
      }
      if ( !strncmp( &line[strlen(line)-16], ".word", 5) ) {
        size = 4;
        return true;
      }

      return false;
    }

    // Check i386 nops
    if ( !strncmp( target, "i386", 4 ) ) {
      if ( isNop ) {
        size = 1; 
        return true;
      }
      // i386 has some two and three byte nops
      if ( !strncmp( &line[strlen(line)-14], "xchg   %ax,%ax", 14) ) {
        size = 2;
        return true;
      }
      if ( !strncmp( &line[strlen(line)-16], "xor    %eax,%eax", 16) ) {
        size = 2;
        return true;
      }
      if ( !strncmp( &line[strlen(line)-16], "xor    %ebx,%ebx", 16) ) {
        size = 2;
        return true;
      }
      if ( !strncmp( &line[strlen(line)-21], "lea    0x0(%esi),%esi", 21) ) {
        size = 3;
        return true;
      }

      return false;
    }

    // Check M68K/Coldfire nops
    if ( !strncmp( target, "m68k", 4 ) ) {
      if ( isNop ) {
        size = 2; 
        return true;
      }

      #define GNU_LD_FILLS_ALIGNMENT_WITH_RTS
      #if defined(GNU_LD_FILLS_ALIGNMENT_WITH_RTS)
        // Until binutils 2.20, binutils would fill with rts not nop
        if ( !strcmp( &line[strlen(line)-3], "rts") ) {
          size = 4; 
          return true;
        } 
      #endif

      return false;
    }

    // Check i386 nops
    if ( !strncmp( target, "powerpc", 7 ) ) {
      if ( isNop ) {
        size = 4; 
        return true;
      }

      return false;
    }

    // Check SPARC nops
    if ( !strncmp( target, "sparc", 5 ) ) {
      if ( isNop ) {
        size = 4; 
        return true;
      }

      if ( !strcmp( &line[strlen(line)-7], "unknown") ) {
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

  bool ObjdumpProcessor::initialize(
    const char      *executable,
    CoverageMapBase *coverage
  )
  {
    FILE *objdumpFile;
    char *cStatus;
    char  buffer[512];
    int   i;

    /*
     * Generate the objdump
     */
    sprintf( buffer, "%s -da --source %s | sed -e \'s/ *$//\' >objdump.tmp",
      Tools->getObjdump(), executable );

    if ( system( buffer ) ) {
      fprintf( stderr, "objdump command (%s) failed\n", buffer );
      exit( -1 );
    }

    /*
     *  read the file and update the coverage map passed in
     */

    objdumpFile = fopen( "objdump.tmp", "r" );
    if ( !objdumpFile ) {
      fprintf(
        stderr,
        "ObjdumpProcessor::ProcessFile - unable to open %s\n",
        "objdump.tmp"
      );
      exit(-1);
    }

    /*
     *  How many bytes is a nop?
     */

    while ( 1 ) {
      ObjdumpLine contents;
      cStatus = fgets( buffer, 512, objdumpFile );
      if ( cStatus == NULL ) {
        break;
      }

      buffer[ strlen(buffer) - 1] = '\0';

      contents.line          = buffer;
      contents.isInstruction = false;
      contents.isNop         = false;
      contents.address       = 0xffffffff;

      // fprintf( stderr, "%08x : ", baseAddress );
      contents.isInstruction = isInstruction( buffer );

      if ( contents.isInstruction ) {
        unsigned long l;
        uint32_t baseAddress;
        sscanf( buffer, "%lx:", &l );
	baseAddress = l;
        contents.address = baseAddress;

        contents.isNop = isNop( buffer, contents.nopSize );
        if ( contents.isNop ) {
          // check the byte immediately before and after the nop
          // if either was executed, then mark NOP as executed. Otherwise,
          // we do not want to split the unexecuted range.
          if ( coverage->wasExecuted( baseAddress - 1 ) ||
               coverage->wasExecuted( baseAddress + contents.nopSize ) ) {
            for ( i=0 ; i < contents.nopSize ; i++ )
	      coverage->setWasExecuted( baseAddress + i );
          }
        }
      }

      Contents.push_back( contents );
    }
    fclose( objdumpFile );

    // Remove temporary file
    (void) system( "rm -f objdump.tmp" );
    return true;
  }

  bool ObjdumpProcessor::writeAnnotated(
    CoverageMapBase *coverage,
    uint32_t         low,
    uint32_t         high,
    const char      *annotated
  )
  {
    FILE *annotatedFile;
    std::list<ObjdumpLine>::iterator it;

    if ( !annotated )
      return false;

    annotatedFile = fopen( annotated, "w" );
    if ( !annotatedFile ) {
      fprintf(
        stderr,
        "ObjdumpProcessor::writeAnnotated - unable to open %s\n",
        annotated
      );
      exit(-1);
    }

    for (it =  Contents.begin() ;
	 it != Contents.end() ;
	 it++ ) {
      bool executed = true;  // assume we do not mark it

      if ( it->isInstruction &&
           it->address >= low && it->address <= high &&
           !coverage->wasExecuted( it->address ) )
        executed = false;

      if ( executed )
        fprintf(annotatedFile, "%s\n", it->line.c_str() );
      else
        fprintf(annotatedFile, "%-76s\t<== NOT EXECUTED\n", it->line.c_str() );
     // bool        isNop;
     // uint32_t    address;
    }
    return true;
  }
}
