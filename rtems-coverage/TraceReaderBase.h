/*
 *  $Id$
 */

/*! @file TraceReaderBase.h
 *  @brief TraceReaderBase Specification
 *
 *  This file contains the specification of the TraceReaderBase class.
 */

#ifndef __TRACE_READER_BASE_H__
#define __TRACE_READER_BASE_H__

#include "CoverageMapBase.h"

namespace Coverage {

  /*! @class TraceReaderBase
   *
   *  This class XXX
   */
  class TraceReaderBase {

  public:

    /*! 
     *  This method is the default constructor of a TraceReaderBase instance.
     */
    TraceReaderBase();

    /*! 
     *  This method is the destructor for a TraceReaderBase instance.
     */
    virtual ~TraceReaderBase();

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

