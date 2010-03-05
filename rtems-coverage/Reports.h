/*
 *   $Id$
 */

/*! @file Reports.h
 *  @brief Reports Specification
 *
 *  This file contains the specification of the Reports methods.  This
 *  collection of methods is used to generate the various reports of
 *  the analysis results.
 */

#ifndef __REPORTS_H__
#define __REPORTS_H__

#include <stdint.h>

namespace Coverage {

  /*!
   *  This method produces an annotated assembly listing report containing
   *  the disassembly of each symbol that was not completely covered.
   *
   *  @param[in] fileName identifies the annotated report file name
   */
  void WriteAnnotatedReport(
    const char* const fileName
  );

  /*!
   *  This method produces a report that contains information about each
   *  uncovered branch statement.
   *
   *  @param[in] fileName identifies the branch report file name
   */
  void WriteBranchReport(
    const char* const fileName
  );

  /*!
   *  This method produces a report that contains information about each
   *  uncovered range of bytes.
   *
   *  @param[in] fileName identifies the coverage report file name
   */
  void WriteCoverageReport(
    const char* const fileName
  );

  /*!
   *  This method produces a summary report that lists each uncovered
   *  range of bytes.
   *
   *  @param[in] fileName identifies the size report file name
   */
  void WriteSizeReport(
    const char* const fileName
  );

}

#endif
