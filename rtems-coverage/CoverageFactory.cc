/*
 *  $Id$
 */

/*! @file CoverageFactory.cc
 *  @brief CoverageFactory Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  XXX
 *
 */


#include "CoverageFactory.h"

#include "CoverageReaderQEMU.h"
#include "CoverageReaderRTEMS.h"
#include "CoverageWriterRTEMS.h"
#include "CoverageReaderSkyeye.h"
#include "CoverageWriterSkyeye.h"
#include "CoverageReaderTSIM.h"
#include "CoverageWriterTSIM.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

Coverage::CoverageFormats_t Coverage::CoverageFormatToEnum(
  const char *format
)
{
  if ( !strcmp(format, "QEMU") )
    return COVERAGE_FORMAT_QEMU;

  if ( !strcmp(format, "RTEMS") )
    return COVERAGE_FORMAT_RTEMS;

  if ( !strcmp(format, "Skyeye") )
    return COVERAGE_FORMAT_SKYEYE;

  if ( !strcmp(format, "TSIM") )
    return COVERAGE_FORMAT_TSIM;

  fprintf(
    stderr,
    "%s is unknown coverage format\n"
    "Supported formats: RTEMS, TSIM, and Skyeye\n",
    format
  );
  exit( 1 );
}

Coverage::CoverageReaderBase *Coverage::CreateCoverageReader(
  CoverageFormats_t format
)
{
  switch (format) {
    case COVERAGE_FORMAT_QEMU:
      return new Coverage::CoverageReaderQEMU();
    case COVERAGE_FORMAT_RTEMS:
      return new Coverage::CoverageReaderRTEMS();
    case COVERAGE_FORMAT_SKYEYE:
      return new Coverage::CoverageReaderSkyeye();
    case COVERAGE_FORMAT_TSIM:
      return new Coverage::CoverageReaderTSIM();
    default:
      break;
  }
  return NULL;
}

Coverage::CoverageWriterBase *Coverage::CreateCoverageWriter(
  CoverageFormats_t format
)
{
  switch (format) {
    case COVERAGE_FORMAT_RTEMS:
      return new Coverage::CoverageWriterRTEMS();
    case COVERAGE_FORMAT_SKYEYE:
      return new Coverage::CoverageWriterSkyeye();
    case COVERAGE_FORMAT_TSIM:
      return new Coverage::CoverageWriterTSIM();
    default:
      break;
  }
  return NULL;
}
