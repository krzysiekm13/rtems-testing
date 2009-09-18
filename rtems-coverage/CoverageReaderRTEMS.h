/*
 *  $Id$
 */

/*! @file CoverageReaderRTEMS.h
 *  @brief CoverageReaderRTEMS Specification
 *
 *  This file contains the specification of the CoverageReaderRTEMS class.
 */

#ifndef __COVERAGE_READER_RTEMS_H__
#define __COVERAGE_READER_RTEMS_H__

#include "CoverageReaderBase.h"

namespace Coverage {

  /*! @class CoverageReaderRTEMS
   *
   *  This class implements the class which reads a coverage map file
   *  produced by RTEMS.  Since the SPARC has 32-bit instructions,
   *  RTEMS produces a file with an integer for each 32-bit word.  The
   *  integer will have the least significant bit if the address
   *  was executed.

@verbatim
TBD
@endverbatim
   */
  class CoverageReaderRTEMS : public CoverageReaderBase {

  public:

    /* Inherit documentation from base class. */
    CoverageReaderRTEMS();

    /* Inherit documentation from base class. */
    virtual ~CoverageReaderRTEMS();

    /* Inherit documentation from base class. */
    bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    );
  };

}
#endif

