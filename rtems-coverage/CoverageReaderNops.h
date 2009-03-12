/*
 *  $Id$
 */

/*! @file CoverageReaderNops.h
 *  @brief CoverageReaderNops Specification
 *
 *  This file contains the specification of the CoverageReaderNops class.
 */

#ifndef __COVERAGE_READER_NOPS_H__
#define __COVERAGE_READER_NOPS_H__

#include "CoverageReaderBase.h"

namespace Coverage {

  /*! @class CoverageReaderNops
   *
   *  This class implements the class which reads an objdump output
   *  file and looks for "nops."  For the purpose of coverage
   *  analysis, nops in the executable may be ignored.  The compiler
   *  may produce nops to align functions on particular alignment
   *  boundaries and the nop between functions can not possibly be
   *  executed.
   */
  class CoverageReaderNops : public CoverageReaderBase {

  public:

    /* Inherit documentation from base class. */
    CoverageReaderNops();

    /* Inherit documentation from base class. */
    virtual ~CoverageReaderNops();

    /* Inherit documentation from base class. */
    bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    );
  };

}
#endif

