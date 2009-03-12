/*
 *  $Id$
 */

/*! @file CoverageReaderNops.cc
 *  @brief CoverageReaderNops Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  the reading of an objdump output file and adding nops to a
 *  coverage map. 
 */

#include "CoverageReaderNops.h"
#include "CoverageMapBase.h"
#include <stdio.h>
#include <stdlib.h>

namespace Coverage {

  CoverageReaderNops::CoverageReaderNops()
  {
  }

  CoverageReaderNops::~CoverageReaderNops()
  {
  }

  bool CoverageReaderNops::ProcessFile(
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
      fprintf( stderr, "CoverageReaderNops::ProcessFile - unable to open %s\n", file );
      exit(-1);
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
          fprintf( stderr, "Error in %s at address 0x%08x\n", file, baseAddress );
	  exit( -1 );
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
