/*
 *  $Id$
 */

/*! @file CoverageReaderSkyeye.cc
 *  @brief CoverageReaderSkyeye Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */

#include "CoverageReaderSkyeye.h"
#include "CoverageMapBase.h"
#include <stdio.h>
#include <stdlib.h>

#include "skyeye_header.h"

namespace Coverage {

  CoverageReaderSkyeye::CoverageReaderSkyeye()
  {
  }

  CoverageReaderSkyeye::~CoverageReaderSkyeye()
  {
  }

  bool CoverageReaderSkyeye::ProcessFile(
    const char      *file,
    CoverageMapBase *coverage
  )
  {
    FILE          *coverageFile;
    int            baseAddress;
    int            length;
    int            status;
    int            i;
    uint8_t        cover;
    prof_header_t  header;

    /*
     *  read the file and update the coverage map passed in
     */
    coverageFile = fopen( file, "r" );
    if ( !coverageFile ) {
      fprintf(
        stderr,
        "CoverageReaderSkyeye::ProcessFile - unable to open %s\n",
        file
      );
      exit(-1);
    }

    status = fread( &header, sizeof(header), 1, coverageFile );
    if ( status != 1 ) {
      fprintf(
        stderr,
        "CoverageReaderSkyeye::ProcessFile - unable to read header "
           "from %s\n",
        file
      );
      exit(-1);
    }

    baseAddress = header.prof_start;
    length      = header.prof_end - header.prof_start;
    
    for ( i=0 ; i<length ; i += 8 ) {
      fread( &cover, sizeof(uint8_t), 1, coverageFile );
      if ( status != 1 ) {
        fprintf(
	  stderr,
	  "CoverageReaderSkyeye::ProcessFile - breaking after %d\n",
	  i
        );
        break;
      }

      if ( cover & 0x01 ) {
        coverage->setWasExecuted( baseAddress + i );
        coverage->setWasExecuted( baseAddress + i + 1 );
        coverage->setWasExecuted( baseAddress + i + 2 );
        coverage->setWasExecuted( baseAddress + i + 3 );
      }
      if ( cover & 0x10 ) {
        coverage->setWasExecuted( baseAddress + i + 4 );
        coverage->setWasExecuted( baseAddress + i + 5 );
        coverage->setWasExecuted( baseAddress + i + 6 );
        coverage->setWasExecuted( baseAddress + i + 7 );
      }
    }

    fclose( coverageFile );
    return true;
  }
}
