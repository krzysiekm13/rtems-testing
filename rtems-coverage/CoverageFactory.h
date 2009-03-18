/*
 *  $Id$
 */

/*! @file CoverageFactory.h
 *  @brief CoverageFactory Specification
 *
 *  This file contains the specification of the CoverageFactory class.
 */
#ifndef __COVERAGE_FACTORY_H__
#define __COVERAGE_FACTORY_H__

#include "CoverageReaderBase.h"
#include "CoverageWriterBase.h"

namespace Coverage {

  typedef enum {
    COVERAGE_FORMAT_TSIM,
    COVERAGE_FORMAT_SKYEYE
  } CoverageFormats_t;

  CoverageFormats_t CoverageFormatToEnum(
    const char *format
  );

  CoverageReaderBase *CreateCoverageReader(
    CoverageFormats_t format
  );

  CoverageWriterBase *CreateCoverageWriter(
    CoverageFormats_t format
  );
}
#endif

