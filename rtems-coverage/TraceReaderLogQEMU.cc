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

#include "app_common.h"
#include "TraceReaderBase.h"
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
    bool                done          = false;
    bool                isBranch      = false;
    QEMU_LOG_IN_Block_t first         = { 0, "", "" };
    QEMU_LOG_IN_Block_t last          = { 0, "", "" };
    QEMU_LOG_IN_Block_t nextExecuted  = { 0, "", "" };
    uint32_t            nextlogical;
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


    //
    //  Discard Header section
    //
    if (! ReadUntilFound( logFile, QEMU_LOG_SECTION_END ) ) {
      fprintf( stderr, "Unable to locate end of log file header\n" );
      return false;
    }

    //
    //  Find first IN block
    //
    if (! ReadUntilFound( logFile, QEMU_LOG_IN_KEY )){
      fprintf(stderr,"Error: Unable to locate first IN: Block in Log file \n");
      return false;
    }

    //
    //  Read First Start Address
    //
    if ( 
      fscanf( 
        logFile, 
        "0x%08lx: %s %s\n", 
        &first.address, 
        first.instruction, 
        first.data 
      ) != 3  ) 
    {
      fprintf(stderr, "Error Unable to Read Initial First Block\n" );
      done = true;
    }

    while (!done) {

        last = first;
   
        // Read until we get to the last instruction in the block.
        while( 
          fscanf( 
             logFile, 
             "0x%08lx: %s %s\n", 
             &last.address, 
             last.instruction, 
             last.data 
           ) == 3 
        );

        // If (last.instruction is a branch instruction) {

        nextlogical = objdumpProcessor->getAddressAfter(last.address);

        if (! ReadUntilFound( logFile, QEMU_LOG_IN_KEY )) {
          done = true;
          nextExecuted = last;
        }else if ( 
          fscanf( 
            logFile, 
            "0x%08lx: %s %s\n", 
            &nextExecuted.address, 
            nextExecuted.instruction, 
            nextExecuted.data 
          ) != 3 
        ) {
          fprintf(stderr, "Error Unable to Read First Block\n" );
        }


        // If the nextlogical was not found we are throwing away
        // the block; otherwise add the block to the trace list.
        if (nextlogical != 0) {
          isBranch = objdumpProcessor->IsBranch( last.instruction ); 

          if (  isBranch &&
               ( nextExecuted.address == nextlogical )) {
            Trace.add( 
              first.address, 
              nextlogical, 
              TraceList::EXIT_REASON_BRANCH_NOT_TAKEN 
            );
          } else if (isBranch) {
            Trace.add( 
              first.address, 
              nextlogical, 
              TraceList::EXIT_REASON_BRANCH_TAKEN 
            );
          } else
            Trace.add(first.address, nextlogical, TraceList::EXIT_REASON_OTHER);
        }    
        first = nextExecuted;
    } 

    fclose( logFile );
    return true;
  }
}
