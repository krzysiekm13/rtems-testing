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

#ifndef __REPORTSHTML_H__
#define __REPORTSHTML_H__

#include <stdint.h>
#include "ReportsBase.h"
#include "Explanations.h"

namespace Coverage {

class ReportsHtml: public ReportsBase {

  public:
    ReportsHtml();
   ~ReportsHtml();

   /*!
    *  This method produces an index file.
    *
    *  @param[in] fileName identifies the file name.
    */
   void WriteIndex(
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

  protected:

    AnnotatedLineState_t lastState_m;

    virtual FILE* OpenBranchFile(
      const char* const fileName,
      bool              hasBranches
    );

    virtual FILE* OpenCoverageFile(
      const char* const fileName
    );

    virtual FILE* OpenSizeFile(
      const char* const fileName
    );

    virtual FILE* OpenSymbolSummaryFile(
      const char* const fileName
    );

    virtual void CloseAnnotatedFile(
      FILE*  aFile
    );

    virtual void CloseBranchFile(
      FILE*  aFile,
      bool   hasBranches
    );

    virtual void CloseCoverageFile(
      FILE*  aFile
    );

    virtual void CloseSizeFile(
      FILE*  aFile
    );

    virtual void CloseSymbolSummaryFile(
      FILE*  aFile
    );

    virtual void PutAnnotatedLine( 
      FILE*                aFile, 
      AnnotatedLineState_t state, 
      std::string          line, 
      uint32_t             id 
    );

    virtual bool PutNoBranchInfo(
      FILE* report
    );

    virtual bool PutBranchEntry(
      FILE*                                            report,
      unsigned int                                     number,
      Coverage::DesiredSymbols::symbolSet_t::iterator  symbolPtr,
      Coverage::CoverageRanges::ranges_t::iterator     rangePtr
    );

    virtual void putCoverageNoRange(
      FILE*        report,
      unsigned int number,
      std::string  symbol
    );

    virtual bool PutCoverageLine(
      FILE*                                           report,
      unsigned int                                    number,
      Coverage::DesiredSymbols::symbolSet_t::iterator ditr,
      Coverage::CoverageRanges::ranges_t::iterator    ritr
    );

    virtual bool PutSizeLine(
      FILE*                                           report,
      unsigned int                                    number,
      Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
      Coverage::CoverageRanges::ranges_t::iterator    range
    );

    virtual bool PutSymbolSummaryLine(
      FILE*                                           report,
      unsigned int                                    number,
      Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
      Coverage::CoverageRanges::ranges_t::iterator    range
    );

    virtual FILE* OpenFile(
      const char* const fileName
    );

    virtual bool WriteExplationFile(
      const char*                  fileName,
      const Coverage::Explanation* explanation
    );
  };

}

#endif
