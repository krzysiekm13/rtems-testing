/*
 *  $Id$
 */

/*! @file CoverageWriterSkyeye.cc
 *  @brief CoverageWriterSkyeye Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */

#include "CoverageWriterSkyeye.h"
#include <stdio.h>
#include <stdlib.h>

namespace Coverage {
  
  CoverageWriterSkyeye::CoverageWriterSkyeye()
  {
  }

  CoverageWriterSkyeye::~CoverageWriterSkyeye()
  {
  }

  
  void CoverageWriterSkyeye::writeFile(
    const char                *file,
    Coverage::CoverageMapBase *coverage,
    uint32_t                   lowAddress,
    uint32_t                   highAddress
  )
  {
    FILE      *coverageFile;
    size_t     length;
    uint32_t   a;
    int        status;
    uint8_t    cover;

    /*
     *  read the file and update the coverage map passed in
     */

    coverageFile = fopen( file, "w" );
    if ( !coverageFile ) {
      fprintf(
        stderr,
        "CoverageWriterSkyeye::ProcessFile - unable to open %s\n",
        file
      );
      exit(-1);
    }

    status = fwrite(&lowAddress, 1, sizeof(lowAddress), coverageFile);
    if (status != sizeof(lowAddress)) {
      fprintf(
        stderr,
        "CoverageWriterSkyeye::ProcessFile - unable to write baseAddress "
           "to %s\n",
        file
      );
      exit(-1);
    }

    length = highAddress - lowAddress;

    status = fwrite(&length, 1, sizeof(length), coverageFile);
    if (status != sizeof(length)) {
      fprintf(
        stderr,
        "CoverageWriterSkyeye::ProcessFile - unable to write length "
           "to %s\n",
        file
      );
      exit(-1);
    }


    for ( a=lowAddress ; a < highAddress ; a+= 8 ) {
      cover  = ((coverage->wasExecuted( a ))     ? 0x01 : 0);
      cover |= ((coverage->wasExecuted( a + 4 )) ? 0x10 : 0);
      status = fprintf( coverageFile, "%d ", cover );
      status = fwrite(&cover, 1, sizeof(cover), coverageFile);
      if (status != sizeof(cover)) {
	fprintf( stderr, "Error writing in %s at address 0x%08x\n", file, a );
	exit( -1 );
      }
      // fprintf( stderr, "0x%x %d\n", a, cover );
    }

    fclose( coverageFile );
  }
}
