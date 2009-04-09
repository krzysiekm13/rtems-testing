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

namespace Coverage {

  class ObjdumpLine {
  
  public:
     ObjdumpLine()
     {
     } 

     ~ObjdumpLine()
     {
     } 

     std::string line;
     bool        isInstruction;
     bool        isNop;
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
    const char *line
  )
  {
    if ( !isInstruction(line) )
      return false;

    if ( !strcmp( &line[strlen(line)-4], "nop ") )
      return true;
    
    if ( !strcmp( &line[strlen(line)-7], "unknown") )
      return true;
    
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

    /*
     * Generate the objdump
     */
    sprintf( buffer, "%s -da --source %s >objdump.tmp",
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
      fprintf( stderr, "ObjdumpProcessor::ProcessFile - unable to open %s\n", "objdump.tmp" );
      exit(-1);
    }

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

        contents.isNop = isNop( buffer );
        if ( contents.isNop ) {
	  coverage->setWasExecuted( baseAddress );
	  coverage->setWasExecuted( baseAddress + 1 );
	  coverage->setWasExecuted( baseAddress + 2 );
	  coverage->setWasExecuted( baseAddress + 3 );
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
