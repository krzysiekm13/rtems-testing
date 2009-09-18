/*
 *  $Id$
 */

/*! @file CoverageReaderRTEMS.cc
 *  @brief CoverageReaderRTEMS Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the RTEMS coverage data files.
 */

#include "CoverageReaderRTEMS.h"
#include "CoverageMapBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "rtemscov_header.h"

namespace Coverage {

  CoverageReaderRTEMS::CoverageReaderRTEMS()
  {
  }

  CoverageReaderRTEMS::~CoverageReaderRTEMS()
  {
  }

  bool CoverageReaderRTEMS::ProcessFile(
    const char      *file,
    CoverageMapBase *coverage
  )
  {
    FILE                        *coverageFile;
    uintptr_t                    baseAddress;
    uintptr_t                    length;
    int                          status;
    uintptr_t                    i;
    uint8_t                      cover;
    rtems_coverage_map_header_t  header;
    struct stat                  statbuf;

    /*
     *  Verify it has a non-zero size
     */
    status = stat( file, &statbuf );
    if ( status == -1 ) {
      fprintf( stderr, "Unable to stat %s\n", file );
      exit( -1 );
    }

    if ( statbuf.st_size == 0 ) {
      fprintf( stderr, "%s is 0 bytes long\n", file );
      exit( -1 );
    }

    /*
     *  read the file and update the coverage map passed in
     */
    coverageFile = fopen( file, "r" );
    if ( !coverageFile ) {
      fprintf(
        stderr,
        "CoverageReaderRTEMS::ProcessFile - unable to open %s\n",
        file
      );
      exit(-1);
    }

    status = fread( &header, sizeof(header), 1, coverageFile );
    if ( status != 1 ) {
      fprintf(
        stderr,
        "CoverageReaderRTEMS::ProcessFile - unable to read header "
           "from %s\n",
        file
      );
      exit(-1);
    }

    baseAddress = header.start;
    length      = header.end - header.start;
    
    #if 0
    fprintf( 
      stderr,
      "%s: 0x%08x 0x%08x 0x%08lx/%ld\n", 
      file,
      header.start,
      header.end,
      (unsigned long) length,
      (unsigned long) length
    );
    #endif
      
    for ( i=0 ; i<length ; i++ ) {
      status = fread( &cover, sizeof(uint8_t), 1, coverageFile );
      if ( status != 1 ) {
        fprintf(
          stderr,
          "CoverageReaderRTEMS::ProcessFile - breaking after 0x%08x in %s\n",
          (unsigned int) i,
          file
        );
        break;
      }

      if ( cover & 0x01 ) {
        coverage->setWasExecuted( baseAddress + i );
      }
    }

    fclose( coverageFile );
    return true;
  }
}
