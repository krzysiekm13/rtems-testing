/*
 *  $Id$
 */

/*! @file CoverageReaderQEMU.h
 *  @brief CoverageReaderQEMU Specification
 *
 *  This file contains the specification of the CoverageReaderQEMU class.
 */

#ifndef __COVERAGE_READER_QEMU_H__
#define __COVERAGE_READER_QEMU_H__

#include "CoverageReaderBase.h"

namespace Coverage {

  /*! @class CoverageReaderQEMU
   *
   *  This class implements the class which reads a coverage map file
   *  produced by QEMU.  Since the SPARC has 32-bit instructions,
   *  QEMU produces a file with an integer for each 32-bit word.  The
   *  integer will have the least significant bit if the address
   *  was executed.

@verbatim
TBD
@endverbatim
   */
  class CoverageReaderQEMU : public CoverageReaderBase {

  public:

    /* Inherit documentation from base class. */
    CoverageReaderQEMU();

    /* Inherit documentation from base class. */
    virtual ~CoverageReaderQEMU();

    /* Inherit documentation from base class. */
    bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    );
  };

}
#endif
