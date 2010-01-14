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
      if ( !strncmp( &line[strlen(line)-16], "xor    %esi,%esi", 16) ) {
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

        coverage->setIsStartOfInstruction( baseAddress );

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
    const char      *annotatedText,
    const char      *annotatedHTML
  )
  {
    FILE *aText;
    FILE *aHTML;
    std::list<ObjdumpLine>::iterator it;

    if ( !annotatedText && !annotatedHTML )
      return false;

    // Open Annotated Text File
    aText = NULL;
    if ( annotatedText ) {
      aText = fopen( annotatedText, "w" );
      if ( !aText ) {
        fprintf(
          stderr,
          "ObjdumpProcessor::writeAnnotated - unable to open %s\n",
          annotatedText
        );
        exit(-1);
      }
    }

    // Open Annotated HTML File
    aHTML = NULL;
    if ( annotatedHTML ) {
      aHTML = fopen( annotatedHTML, "w" );
      if ( !aHTML ) {
        fprintf(
          stderr,
          "ObjdumpProcessor::writeAnnotated - unable to open %s\n",
          annotatedHTML
        );
        exit(-1);
      }

      // WRITE HTML FILE HEADER
      if ( aHTML ) {
        fprintf(
          aHTML,
          "<HTML>\n"
          "<HEAD>\n"
          /* XXX more specific */
          "<TITLE>RTEMS Coverage Annotated Dump</TITLE>\n"
          "<H1>RTEMS Coverage Annotated Dump</H1>\n"
          "</HEAD>\n"
          "<BODY bgcolor=#ffffff>\n"
          "\n"
        );
      }
    }

    for (it =  Contents.begin() ; it != Contents.end() ; it++ ) {
      const char *annotation = "";
      if ( it->isInstruction &&
           it->address >= low && it->address <= high )  {
        
        if ( coverage->wasExecuted( it->address ) )
          annotation = "\t<== NOT EXECUTED";
        else if ( coverage->isBranch( it->address ) ) {
          if ( coverage->wasAlwaysTaken( it->address ) )
            annotation = "\t<== ALWAYS TAKEN";
          else if ( coverage->wasNeverTaken( it->address ) )
            annotation = "\t<== NEVER TAKEN";
        }

      }
      if ( aText ) {
        fprintf( aText, "%-76s%s\n", it->line.c_str(), annotation );
      }
      if ( aHTML ) {
        // XXX WRITE HTML FILE INSTRUCTION LINE with address as the "tag"
        //     <a name="0x00001234" id="0x00001234">0x00001234</a>
        //     This will require further decomposition of the objdump line
        //     into address and contents.
        fprintf(
          aHTML,
          "%-76s%s\n",
          it->line.c_str(),
          annotation
        );
      }
    }

    if ( aText )
      fclose( aText );

    if ( aHTML ) {
      // XXX WRITE HTML FILE TRAILER
      fprintf(
        aHTML,
        "</BODY>\n"
        "</HTML>\n"
      );
      fclose( aHTML );
    }
    return true;
  }
}
