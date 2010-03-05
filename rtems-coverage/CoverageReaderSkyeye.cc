/*
 *  $Id$
 */

/*! @file CoverageReaderSkyeye.cc
 *  @brief CoverageReaderSkyeye Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the Skyeye coverage data files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "CoverageReaderSkyeye.h"
#include "ExecutableInfo.h"
#include "skyeye_header.h"

namespace Coverage {

  CoverageReaderSkyeye::CoverageReaderSkyeye()
  {
  }

  CoverageReaderSkyeye::~CoverageReaderSkyeye()
  {
  }

  bool CoverageReaderSkyeye::processFile(
    const char* const     file,
    ExecutableInfo* const executableInformation
  )
  {
    uintptr_t     baseAddress;
    uint8_t       cover;
    FILE*         coverageFile;
    prof_header_t header;
    uintptr_t     i;
    uintptr_t     length;
    struct stat   statbuf;
    int           status;

    //
    // Verify that the coverage file has a non-zero size.
    //
    status = stat( file, &statbuf );
    if (status == -1) {
      fprintf( stderr, "Unable to stat %s\n", file );
      return false;
    }

    if (statbuf.st_size == 0) {
      fprintf( stderr, "%s is 0 bytes long\n", file );
      return false;
    }

    //
    // Open the coverage file and read the header.
    //
    coverageFile = fopen( file, "r" );
    if (!coverageFile) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

    status = fread( &header, sizeof(header), 1, coverageFile );
    if (status != 1) {
      fprintf( stderr, "Unable to read header from %s\n", file );
      return false;
    }

    baseAddress = header.prof_start;
    length      = header.prof_end - header.prof_start;
    
    #if 0
    fprintf( 
      stderr,
      "%s: 0x%08x 0x%08x 0x%08lx/%ld\n", 
      file,
      header.prof_start,
      header.prof_end,
      (unsigned long) length,
      (unsigned long) length
    );
    #endif

    //
    // Read and process each line of the coverage file.
    //
    for (i=0; i<length; i+=8) {
      status = fread( &cover, sizeof(uint8_t), 1, coverageFile );
      if (status != 1) {
        fprintf(
	  stderr,
	  "CoverageReaderSkyeye::ProcessFile - breaking after 0x%08x in %s\n",
	  (unsigned int) i,
          file
        );
        break;
      }

      if (cover & 0x01) {
        executableInformation->markWasExecuted( baseAddress + i );
        executableInformation->markWasExecuted( baseAddress + i + 1 );
        executableInformation->markWasExecuted( baseAddress + i + 2 );
        executableInformation->markWasExecuted( baseAddress + i + 3 );
      }
      if (cover & 0x10) {
        executableInformation->markWasExecuted( baseAddress + i + 4 );
        executableInformation->markWasExecuted( baseAddress + i + 5 );
        executableInformation->markWasExecuted( baseAddress + i + 6 );
        executableInformation->markWasExecuted( baseAddress + i + 7 );
      }
    }

    fclose( coverageFile );
    return true;
  }
}
