/*
 *  $Id$
 */

/*! @file TraceReaderSKYEYE.h
 *  @brief TraceReaderQEMU Specification
 *
 *  This file contains the specification of the TraceReaderQEMU class.
 */

#ifndef __COVERAGE_READER_QEMU_H__
#define __COVERAGE_READER_QEMU_H__

#include "TraceReaderBase.h"

namespace Coverage {

  /*! @class TraceReaderQEMU
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
  class TraceReaderQEMU : public TraceReaderBase {

  public:

    /* Inherit documentation from base class. */
    TraceReaderQEMU();

    /* Inherit documentation from base class. */
    virtual ~TraceReaderQEMU();

    /* Inherit documentation from base class. */
    bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    );
  };

}
#endif

