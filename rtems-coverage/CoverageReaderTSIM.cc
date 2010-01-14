/*
 *  $Id$
 */

/*! @file CoverageReaderTSIM.cc
 *  @brief CoverageReaderTSIM Implementation
 *
 *  This file contains the implementation of the CoverageReader class
 *  for the coverage files written by the SPARC simulator TSIM.
 */

#include "CoverageReaderTSIM.h"
#include "CoverageMapBase.h"
#include <stdio.h>
#include <stdlib.h>

namespace Coverage {

  CoverageReaderTSIM::CoverageReaderTSIM()
  {
  }

  CoverageReaderTSIM::~CoverageReaderTSIM()
  {
  }

  bool CoverageReaderTSIM::ProcessFile(
    const char      *file,
    CoverageMapBase *coverage
  )
  {
    FILE *coverageFile;
    int   baseAddress;
    int   status;
    int   i;
    int   cover;

    /*
     *  read the file and update the coverage map passed in
     */
    coverageFile = fopen( file, "r" );
    if ( !coverageFile ) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

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
          coverage->setWasExecuted( baseAddress + i );
          coverage->setWasExecuted( baseAddress + i + 1 );
          coverage->setWasExecuted( baseAddress + i + 2 );
          coverage->setWasExecuted( baseAddress + i + 3 );
        }
      }
      // fprintf( stderr, "\n" );
  
    }

    fclose( coverageFile );
    return true;
  }
}
