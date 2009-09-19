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

#include "TraceReaderQEMU.h"

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

    if ( !strcmp(format, "qemu") )
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
      case TRACE_FORMAT_QEMU:
        return new Coverage::TraceReaderQEMU();
      default:
        break;
    }
    return NULL;
  }
}

