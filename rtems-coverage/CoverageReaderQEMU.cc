/*
 *  $Id$
 */

/*! @file CoverageReaderQEMU.cc
 *  @brief CoverageReaderQEMU Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the QEMU coverage data files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "CoverageReaderQEMU.h"
#include "ExecutableInfo.h"
#include "CoverageMap.h"

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

namespace Coverage {

  CoverageReaderQEMU::CoverageReaderQEMU()
  {
  }

  CoverageReaderQEMU::~CoverageReaderQEMU()
  {
  }

  bool CoverageReaderQEMU::processFile(
    const char* const     file,
    ExecutableInfo* const executableInformation
  )
  {
    struct trace_entry  entry;
    struct trace_header header;
    uintptr_t           i;
    struct STAT         statbuf;
    int                 status;
    FILE*               traceFile;

    //
    // Verify that the coverage file has a non-zero size.
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
    // Open the coverage file and read the header.
    //
    traceFile = OPEN( file, "r" );
    if (!traceFile) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

    status = fread( &header, sizeof(trace_header), 1, traceFile );
    if (status != 1) {
      fprintf( stderr, "Unable to read header from %s\n", file );
      return false;
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

    //
    // Read and process each line of the coverage file.
    //
    while (1) {
      CoverageMapBase* aCoverageMap = NULL;

      status = fread( &entry, sizeof(struct trace_entry), 1, traceFile );
      if (status != 1)
        break;
      #if 0
        fprintf( stderr, "0x%08x %d 0x%2x\n", entry.pc, entry.size, entry.op );
      #endif

      // Mark block as fully executed.
      // Obtain the coverage map containing the specified address.
      aCoverageMap = executableInformation->getCoverageMap( entry.pc );

      // Ensure that coverage map exists.
      if (!aCoverageMap)
        continue;

      if (entry.op & TRACE_OP_BLOCK) {
       for (i=0; i<entry.size; i++) {
          aCoverageMap->setWasExecuted( entry.pc + i );
        }
      }

      // Determine if additional branch information is available. */
      if (entry.op & 0x0f) {

        if (entry.op & TRACE_OP_TAKEN)
          aCoverageMap->setWasTaken( entry.pc + entry.size - 1);

        else if (entry.op & TRACE_OP_NOT_TAKEN)
          aCoverageMap->setWasNotTaken( entry.pc + entry.size -1 );
      }
    }

    fclose( traceFile );
    return true;
  }
}
