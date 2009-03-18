/*
 *  $Id$
 */

/*! @file CoverageReaderSKYEYE.h
 *  @brief CoverageReaderSkyeye Specification
 *
 *  This file contains the specification of the CoverageReaderSkyeye class.
 */

#ifndef __COVERAGE_READER_Skyeye_H__
#define __COVERAGE_READER_Skyeye_H__

#include "CoverageReaderBase.h"

namespace Coverage {

  /*! @class CoverageReaderSkyeye
   *
   *  This class implements the class which reads a coverage map file
   *  produced by Skyeye.  Since the SPARC has 32-bit instructions,
   *  Skyeye produces a file with an integer for each 32-bit word.  The
   *  integer will have the least significant bit if the address
   *  was executed.

@verbatim
TBD
@endverbatim
   */
  class CoverageReaderSkyeye : public CoverageReaderBase {

  public:

    /* Inherit documentation from base class. */
    CoverageReaderSkyeye();

    /* Inherit documentation from base class. */
    virtual ~CoverageReaderSkyeye();

    /* Inherit documentation from base class. */
    bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    );
  };

}
#endif

