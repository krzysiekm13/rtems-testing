/*
 *  $Id$
 */

/*! @file TraceFactory.cc
 *  @brief TraceFactory Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */


#include "TraceFactory.h"

// #include "TraceReaderQemu.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

namespace Coverage {
  TraceFormats_t TraceFormatToEnum(
    const char *format
  )
  {
    if ( !strcmp(format, "QEMU") )
      return TRACE_FORMAT_QEMU;

    fprintf(
      stderr,
      "%s is unknown coverage format\n"
      "Supported formats: QEMU\n",
      format
    );
    exit( 1 );
  }

  TraceReaderBase *CreateTraceReader(
    TraceFormats_t format
  )
  {
    switch (format) {
  #if 0
      case TRACE_FORMAT_QEMU:
        return new Coverage::TraceReaderQemu();
  #endif
      default:
        break;
    }
    return NULL;
  }
}

