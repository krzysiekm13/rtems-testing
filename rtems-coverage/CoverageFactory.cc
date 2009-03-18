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
  if ( !strcmp(format, "TSIM") )
    return COVERAGE_FORMAT_TSIM;

  if ( !strcmp(format, "Skyeye") )
    return COVERAGE_FORMAT_SKYEYE;

  fprintf(
    stderr,
    "%s is unknown coverage format\n"
    "Supported formats: TSIM and Skyeye\n",
    format
  );
  exit( 1 );
}

Coverage::CoverageReaderBase *Coverage::CreateCoverageReader(
  CoverageFormats_t format
)
{
  switch (format) {
    case COVERAGE_FORMAT_TSIM:
      return new Coverage::CoverageReaderTSIM();
    case COVERAGE_FORMAT_SKYEYE:
      return new Coverage::CoverageReaderSkyeye();
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
    case COVERAGE_FORMAT_TSIM:
      return new Coverage::CoverageWriterTSIM();
    case COVERAGE_FORMAT_SKYEYE:
      return new Coverage::CoverageWriterSkyeye();
    default:
      break;
  }
  return NULL;
}

