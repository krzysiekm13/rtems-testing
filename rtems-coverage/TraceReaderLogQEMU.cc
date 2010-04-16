/*
 *  $Id$
 */

/*! @file TraceReaderLogQEMU.cc
 *  @brief TraceReaderLogQEMU Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the QEMU coverage data files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "qemu-log.h"

#include "TraceReaderBase.cc"
#include "TraceReaderLogQEMU.h"
#include "TraceList.h"

/* XXX really not always right */
typedef uint32_t target_ulong;

#include "qemu-traces.h"

/* hack so this can compile on the RH7 RTEMS 4.5 host */
#if (__GNUC__ <= 2)
#define STAT stat
#define OPEN fopen
#else
#define STAT stat64
#define OPEN fopen64
#endif


namespace Trace {


bool ReadUntilFound( FILE *file, const char *line )
{
  char discardBuff[100];
  size_t  len = strlen( line );

  do { 
    if (! fgets( discardBuff, 99, file ) )
      return false;

    if ( strncmp( discardBuff, line, len ) == 0 ) 
      return true; 
  } while (1);
}


  TraceReaderLogQEMU::TraceReaderLogQEMU()
  {
  }

  TraceReaderLogQEMU::~TraceReaderLogQEMU()
  {
  }

  bool TraceReaderLogQEMU::processFile(
    const char* const     file
  )
  {
    struct trace_header header;
    uintptr_t           i;
    struct STAT         statbuf;
    int                 status;
    FILE*               logFile;

    //
    // Verify that the log file has a non-zero size.
    //
    // NOTE: We prefer stat64 because some of the coverage files are HUGE!
    status = STAT( file, &statbuf );
    if (status == -1) {
      fprintf( stderr, "Unable to stat %s\n", file );
      return false;
    }

    if (statbuf.st_size == 0) {
      fprintf( stderr, "%s is 0 bytes long\n", file );
      return false;
    }

    //
    // Open the coverage file and discard the header.
    //
    logFile = OPEN( file, "r" );
    if (!logFile) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

    if (! ReadUntilFound( logFile, QEMU_LOG_SECTION_END ) ) {
      fprintf( stderr, "Unable to locate end of log file header\n" );
      return false;
    }

    do {

      QEMU_LOG_IN_Block_t first, last;

      if (! ReadUntilFound( logFile, QEMU_LOG_IN_KEY ))
        break;
     
       if ( fscanf( logFile, "0x%08x: %s %s\n", &first.address, first.instruction, first.data ) != 3 )
         fprintf(stderr, "Error Unable to Read First Block\n" );
printf("Start Address 0x%x, %s %s\n", first.address, first.instruction, first.data );

        last = first;
   
        while( fscanf( logFile, "0x%08x: %s %s\n", &last.address, last.instruction, last.data ) == 3 );
printf("End Address 0x%x, %s %s\n", last.address, last.instruction, last.data ); 

    } while (1);

    //
    // Read and process each line of the coverage file.
    //

    fclose( logFile );
    return true;
  }
}




main ()
{
  Trace::TraceReaderLogQEMU log;

  log.processFile( "/tmp/qemu.log" );
}
