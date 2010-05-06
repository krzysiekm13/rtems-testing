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

#ifndef __REPORTSBASE_H__
#define __REPORTSBASE_H__

#include <stdint.h>
#include <string>
#include "DesiredSymbols.h"

namespace Coverage {


class ReportsBase {

  public:
    ReportsBase();
   ~ReportsBase();

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


  std::string ReportExtension() { return reportExtension_m; }

  protected:

    typedef enum {
      A_SOURCE,
      A_EXECUTED,
      A_NEVER_EXECUTED,
      A_BRANCH_TAKEN,
      A_BRANCH_NOT_TAKEN
    }AnnotatedLineState_t;

    std::string reportExtension_m;


    FILE* OpenFile(
      const char* const fileName
    );

    virtual FILE* OpenAnnotatedFile(
      const char* const fileName
    );

    virtual FILE* OpenBranchFile(
      const char* const fileName
    );

    virtual FILE* OpenCoverageFile(
      const char* const fileName
    );

    virtual FILE* OpenSizeFile(
      const char* const fileName
    );

    void CloseFile(
      FILE*  aFile
    );

    virtual void CloseAnnotatedFile(
      FILE*  aFile
    );

    virtual void CloseBranchFile(
      FILE*  aFile
    );

    virtual void CloseCoverageFile(
      FILE*  aFile
    );

    virtual void CloseSizeFile(
      FILE*  aFile
    );

    virtual void PutAnnotatedLine( 
      FILE*                aFile, 
      AnnotatedLineState_t state, 
      std::string          line 
    )=0;

    virtual bool PutNoBranchInfo(
      FILE* report
    ) = 0;

    virtual bool PutBranchEntry(
      FILE*                                       report,
      Coverage::DesiredSymbols::symbolSet_t::iterator  symbolPtr,
      Coverage::CoverageRanges::ranges_t::iterator     rangePtr
    )=0;

    virtual void putCoverageNoRange(
      FILE*       report,
      std::string symbol
    )=0;

    virtual bool PutCoverageLine(
      FILE*                                      report,
      Coverage::DesiredSymbols::symbolSet_t::iterator ditr,
      Coverage::CoverageRanges::ranges_t::iterator    ritr
    )=0;

    virtual bool PutSizeLine(
      FILE*                                      report,
      Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
      Coverage::CoverageRanges::ranges_t::iterator    range
    )=0;

};

void GenerateReports();

}

#endif
