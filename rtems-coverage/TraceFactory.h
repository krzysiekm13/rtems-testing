/*
 *  $Id$
 */

/*! @file TraceFactory.h
 *  @brief TraceFactory Specification
 *
 *  This file contains the specification of the TraceFactory class.
 */
#ifndef __TRACE_FACTORY_H__
#define __TRACE_FACTORY_H__

#include "TraceReaderBase.h"

namespace Coverage {

  typedef enum {
    TRACE_FORMAT_QEMU
  } TraceFormats_t;

  TraceFormats_t TraceFormatToEnum(
    const char *format
  );

  TraceReaderBase *CreateTraceReader(
    TraceFormats_t format
  );
}
#endif

