/*
 *  $Id$
 */

/*! @file CoverageReaderTSIM.cc
 *  @brief CoverageReaderTSIM Implementation
 *
 *  This file contains the implementation of the CoverageReader class
 *  for the coverage files written by the SPARC simulator TSIM.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "CoverageReaderTSIM.h"
#include "ExecutableInfo.h"

namespace Coverage {

  CoverageReaderTSIM::CoverageReaderTSIM()
  {
  }

  CoverageReaderTSIM::~CoverageReaderTSIM()
  {
  }

  bool CoverageReaderTSIM::processFile(
    const char* const     file,
    ExecutableInfo* const executableInformation
  )
  {
    int         baseAddress;
    int         cover;
    FILE*       coverageFile;
    int         i;
    struct stat statbuf;
    int         status;

    //
    // Verify that the coverage file has a non-zero size.
    //
    status = stat( file, &statbuf );
    if ( status == -1 ) {
      fprintf( stderr, "Unable to stat %s\n", file );
      return false;
    }

    if ( statbuf.st_size == 0 ) {
      fprintf( stderr, "%s is 0 bytes long\n", file );
      return false;
    }

    //
    // Open the coverage file.
    //
    coverageFile = fopen( file, "r" );
    if ( !coverageFile ) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

    //
    // Read and process each line of the coverage file.
    //
    while ( 1 ) {
      status = fscanf( coverageFile, "%x : ", &baseAddress );
      if ( status == EOF || status == 0 ) {
        break;
      }
      // fprintf( stderr, "%08x : ", baseAddress );
      for ( i=0 ; i < 0x80 ; i+=4 ) {
        status = fscanf( coverageFile, "%d", &cover );
	if ( status == EOF || status == 0 ) {
          fprintf(
            stderr,
            "CoverageReaderTSIM: WARNING! Short line in %s at address 0x%08x\n",
            file,
            baseAddress
          );
          break;
	}
        // fprintf( stderr, "%d ", cover );
        if ( cover & 1 ) {
          executableInformation->markWasExecuted( baseAddress + i );
          executableInformation->markWasExecuted( baseAddress + i + 1 );
          executableInformation->markWasExecuted( baseAddress + i + 2 );
          executableInformation->markWasExecuted( baseAddress + i + 3 );
        }
      }
      // fprintf( stderr, "\n" );
  
    }

    fclose( coverageFile );
    return true;
  }
}
