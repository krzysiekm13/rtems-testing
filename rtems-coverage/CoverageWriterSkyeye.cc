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
#include <string.h>

#include "skyeye_header.h"

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
    FILE          *coverageFile;
    uint32_t       a;
    int            status;
    uint8_t        cover;
    prof_header_t  header;

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

    /* clear out the header and fill it in */
    memset( &header, 0, sizeof(header) );
    header.ver           = 0x1;
    header.header_length = sizeof(header);
    header.prof_start    = lowAddress;
    header.prof_end      = highAddress;
    strcpy( header.desc, "Skyeye Coverage Data" );

    status = fwrite(&header, 1, sizeof(header), coverageFile);
    if (status != sizeof(header)) {
      fprintf(
        stderr,
        "CoverageWriterSkyeye::ProcessFile - unable to write header "
           "to %s\n",
        file
      );
      exit(-1);
    }

    for ( a=lowAddress ; a < highAddress ; a+= 8 ) {
      cover  = ((coverage->wasExecuted( a ))     ? 0x01 : 0);
      cover |= ((coverage->wasExecuted( a + 4 )) ? 0x10 : 0);
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
