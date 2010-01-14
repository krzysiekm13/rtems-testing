/*
 *  $Id$
 */

/*! @file CoverageReaderBase.h
 *  @brief CoverageReaderBase Specification
 *
 *  This file contains the specification of the CoverageReaderBase class.
 */

#ifndef __COVERAGE_READER_BASE_H__
#define __COVERAGE_READER_BASE_H__

#include "CoverageMapBase.h"

namespace Coverage {

  /*! @class CoverageReaderBase
   *
   *  This is the specification of the CoverageReader base class.
   *  All CoverageReader implementations inherit from this.
   */
  class CoverageReaderBase {

  public:

    /*! 
     *  This method is the default constructor of a CoverageReaderBase instance.
     */
    CoverageReaderBase();

    /*! 
     *  This method is the destructor for a CoverageReaderBase instance.
     */
    virtual ~CoverageReaderBase();

    /*!
     *  Process the input @a file and add it to the @a coverage map.
     *
     *  @param[in] file is the file to process
     *  @param[in] coverage is the coverage map to update
     *
     *  @return This method returns TRUE if the method succeeded 
     *  and FALSE if it failed.
     */
    virtual bool ProcessFile(
      const char      *file,
      CoverageMapBase *coverage
    ) = 0;
  };

}
#endif

