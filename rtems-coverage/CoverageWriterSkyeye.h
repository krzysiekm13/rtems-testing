/*
 *  $Id$
 */

/*! @file CoverageWriterSKYEYE.h
 *  @brief CoverageWriterSkyeye Specification
 *
 *  This file contains the specification of the CoverageWriterSkyeye class.
 */

#ifndef __COVERAGE_WRITER_Skyeye_H__
#define __COVERAGE_WRITER_Skyeye_H__

#include "CoverageWriterBase.h"

namespace Coverage {

  /*! @class CoverageWriterSkyeye
   *
   *  This class writes a coverage map in Skyeye format.  The format is 
   *  documented in CoverageReaderSkyeye.
   */
  class CoverageWriterSkyeye : public CoverageWriterBase {

  public:

    /*! 
     *  This method is the default constructor of a CoverageWriterSkyeye instance.
     */
    CoverageWriterSkyeye();

    /*! 
     *  This method is the destructor for a CoverageWriterSkyeye instance.
     */
    virtual ~CoverageWriterSkyeye();

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

