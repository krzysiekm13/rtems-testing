/*
 *  $Id$
 */

/*! @file TraceReaderQEMU.cc
 *  @brief TraceReaderQEMU Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the QEMU coverage data files.
 */

#include "TraceReaderQEMU.h"
#include "CoverageMapBase.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

/* XXX really not always right */
typedef uint32_t target_ulong;

#include "qemu-traces.h"

namespace Coverage {

  TraceReaderQEMU::TraceReaderQEMU()
  {
  }

  TraceReaderQEMU::~TraceReaderQEMU()
  {
  }

  bool TraceReaderQEMU::ProcessFile(
    const char      *file,
    CoverageMapBase *coverage
  )
  {
    FILE               *traceFile;
    int                 status;
    uintptr_t           i;
    struct trace_header header;
    struct trace_entry  entry;
    struct stat         statbuf;

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
    traceFile = fopen( file, "r" );
    if ( !traceFile ) {
      fprintf(
        stderr,
        "TraceReaderQEMU::ProcessFile - unable to open %s\n",
        file
      );
      exit(-1);
    }

    status = fread( &header, sizeof(trace_header), 1, traceFile );
    if ( status != 1 ) {
      fprintf(
        stderr,
        "TraceReaderQEMU::ProcessFile - unable to read header "
           "from %s\n",
        file
      );
      exit(-1);
    }

    #if 0
      fprintf(
        stderr,
        "magic = %s\n"
        "version = %d\n"
        "kind = %d\n"
        "sizeof_target_pc = %d\n"
        "big_endian = %d\n"
        "machine = %02x:%02x\n",
        header.magic,
        header.version,
        header.kind,
        header.sizeof_target_pc,
        header.big_endian,
        header.machine[0], header.machine[1]
       );
    #endif

    while (1) {
      status = fread( &entry, sizeof(struct trace_entry), 1, traceFile );
      if ( status != 1 )
        break;
      #if 0
        fprintf( stderr, "0x%08x %d 0x%2x\n", entry.pc, entry.size, entry.op );
      #endif
      if ( entry.op & TRACE_OP_BLOCK ) { /* Block fully executed.  */
        for ( i=0 ; i<entry.size ; i++ ) {
          coverage->setWasExecuted( entry.pc + i );
        }
      }
    }

    fclose( traceFile );
    return true;
  }
}
