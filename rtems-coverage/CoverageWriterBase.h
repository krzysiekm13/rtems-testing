/*
 *  $Id$
 */

/*! @file CoverageWriterBase.h
 *  @brief CoverageWriterBase Specification
 *
 *  This file contains the specification of the CoverageWriterBase class.
 */

#ifndef __COVERAGE_WRITER_BASE_H__
#define __COVERAGE_WRITER_BASE_H__

#include <stdint.h>
#include "CoverageMapBase.h"

namespace Coverage {

  /*! @class CoverageWriterBase
   *
   *  This class XXX
   */
  class CoverageWriterBase {

  public:

    /*! 
     *  This method is the default constructor of a CoverageWriterBase instance.
     */
    CoverageWriterBase();

    /*! 
     *  This method is the destructor for a CoverageWriterBase instance.
     */
    virtual ~CoverageWriterBase();

    /*!
     *  This method writes the @a coverage map for the specified
     *  address range and writes it to @file.
     *
     *  @param[in] file is the name of the file to write
     *  @param[in] coverage is the coverage map to write
     *  @param[in] lowAddress is the lowest address in the range to process
     *  @param[in] highAddress is the highest address in the range to process
     *
     *  @return This method returns TRUE if the method succeeded 
     *  and FALSE if it failed.
     */
    virtual void writeFile(
      const char                *file,
      Coverage::CoverageMapBase *coverage,
      uint32_t                   lowAddress,
      uint32_t                   highAddress
    ) = 0;
  };

}
#endif

