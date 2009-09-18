/*
 *  $Id$
 */

/*! @file CoverageWriterRTEMS.h
 *  @brief CoverageWriterRTEMS Specification
 *
 *  This file contains the specification of the CoverageWriterRTEMS class.
 */

#ifndef __COVERAGE_WRITER_RTEMS_H__
#define __COVERAGE_WRITER_RTEMS_H__

#include "CoverageWriterBase.h"

namespace Coverage {

  /*! @class CoverageWriterRTEMS
   *
   *  This class writes a coverage map in RTEMS format.  The format is 
   *  documented in CoverageReaderRTEMS.
   */
  class CoverageWriterRTEMS : public CoverageWriterBase {

  public:

    /*! 
     *  This method is the default constructor of a CoverageWriterRTEMS instance.
     */
    CoverageWriterRTEMS();

    /*! 
     *  This method is the destructor for a CoverageWriterRTEMS instance.
     */
    virtual ~CoverageWriterRTEMS();

    /* Inherit documentation from base class. */
    void writeFile(
      const char                *file,
      Coverage::CoverageMapBase *coverage,
      uint32_t                   lowAddress,
      uint32_t                   highAddress
    );
  };

}
#endif

