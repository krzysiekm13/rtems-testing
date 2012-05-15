/*! @file TraceWriterQEMU.cc
 *  @brief TraceWriterQEMU Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the QEMU coverage data files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "app_common.h"
#include "TraceWriterQEMU.h"
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

namespace Trace {

  TraceWriterQEMU::TraceWriterQEMU():
    TraceWriterBase()
  {
  }

  TraceWriterQEMU::~TraceWriterQEMU()
  {
  }

  bool TraceWriterQEMU::writeFile(
    const char* const          file,
    Trace::TraceReaderBase    *log
  )
  {
    struct trace_header header;
    int                 status;
    FILE*               traceFile;
    uint8_t             taken;
    uint8_t             notTaken;

    taken    = TargetInfo->qemuTakenBit();
    notTaken = TargetInfo->qemuNotTakenBit();

    //
    // Verify that the TraceList has a non-zero size.
    //
    if ( log->Trace.set.begin() == log->Trace.set.end() ){
      fprintf( stderr, "ERROR: Empty TraceList\n" );
      return false;
    }

    //
    // Open the trace file.
    //
    traceFile = OPEN( file, "w" );
    if (!traceFile) {
      fprintf( stderr, "Unable to open %s\n", file );
      return false;
    }

    //
    //  Write the Header to the file
    //
    sprintf( header.magic, "%s", QEMU_TRACE_MAGIC );
    header.version = QEMU_TRACE_VERSION;
    header.kind    = QEMU_TRACE_KIND_RAW;  // XXX ??
    header.sizeof_target_pc = 32;
    header.big_endian = false;
    header.machine[0] = 0; // XXX ??
    header.machine[1] = 0; // XXX ??
    status = fwrite( &header, sizeof(trace_header), 1, traceFile );
    if (status != 1) {
      fprintf( stderr, "Unable to write header to %s\n", file );
      return false;
    }

    if (Verbose) 
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

    //
    // Loop through log and write each entry.
    //
    struct trace_entry32  entry;
    TraceList::ranges_t::iterator   itr;

    for (itr = log->Trace.set.begin(); (itr != log->Trace.set.end()); itr++ ){
      entry.pc   = itr->lowAddress;
      entry.size = itr-> length;
      entry.op   = TRACE_OP_BLOCK;
      switch (itr->exitReason) {
        case TraceList::EXIT_REASON_BRANCH_TAKEN:
          entry.op |= taken;
          break;
        case TraceList::EXIT_REASON_BRANCH_NOT_TAKEN:
          entry.op |= notTaken;
          break;
        case TraceList::EXIT_REASON_OTHER:
          break;
        default:
          fprintf(stderr, "Unknown exit Reason\n");
          exit(1);
          break;
       }
       
      if ( Verbose )
        fprintf(stderr, "%x %x %x\n", entry.pc, entry.size, entry.op);

      status = fwrite( &entry, sizeof(entry), 1, traceFile );
      if (status != 1) {
        fprintf( stderr, "Unable to emtry to %s\n", file );
        return false;
      }
    }

    fclose( traceFile );
    return true;
  }
}
