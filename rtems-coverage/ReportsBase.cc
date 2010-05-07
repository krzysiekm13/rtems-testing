/*
 *   $Id$
 */

#include <stdio.h>
#include <string.h>

#include "ReportsBase.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "DesiredSymbols.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"


#include "ReportsText.h"
#include "ReportsHtml.h"

namespace Coverage {

ReportsBase::ReportsBase():
  reportExtension_m("")
{
}

ReportsBase::~ReportsBase()
{
}


FILE* ReportsBase::OpenFile(
  const char* const fileName
)
{
  FILE*  aFile;

  // Open the file.
  aFile = fopen( fileName, "w" );
  if ( !aFile ) {
    fprintf(
      stderr, "Unable to open %s\n", fileName
    );
  }
  return aFile;
}

FILE* ReportsBase::OpenAnnotatedFile(
  const char* const fileName
)
{
  return OpenFile(fileName);
}

FILE* ReportsBase::OpenBranchFile(
  const char* const fileName
)
{
  return OpenFile(fileName);
}

FILE* ReportsBase::OpenCoverageFile(
  const char* const fileName
)
{
  return OpenFile(fileName);
}

FILE* ReportsBase::OpenSizeFile(
  const char* const fileName
)
{
  return OpenFile(fileName);
}


void ReportsBase::CloseFile(
  FILE*  aFile
)
{
  fclose( aFile );
}

void ReportsBase::CloseAnnotatedFile(
  FILE*  aFile
)
{
  CloseFile( aFile );
}

void ReportsBase::CloseBranchFile(
  FILE*  aFile
)
{
  CloseFile( aFile );
}

void  ReportsBase::CloseCoverageFile(
  FILE*  aFile
)
{
  CloseFile( aFile );
}

void  ReportsBase::CloseSizeFile(
  FILE*  aFile
)
{
  CloseFile( aFile );
}


/*
 *  Write annotated report
 */
void ReportsBase::WriteAnnotatedReport(
  const char* const fileName
) {
  FILE*                                           aFile = NULL;
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  std::list<Coverage::ObjdumpProcessor::objdumpLine_t>* theInstructions;
  std::list<Coverage::ObjdumpProcessor::objdumpLine_t>::iterator itr;
  Coverage::CoverageMapBase*                      theCoverageMap = NULL;
  uint32_t                                        bAddress = 0;
  AnnotatedLineState_t                            state;

  aFile = OpenAnnotatedFile(fileName);
  if (!aFile)
    return;

  // Process uncovered branches for each symbol.
  for (ditr = SymbolsToAnalyze->set.begin();
       ditr != SymbolsToAnalyze->set.end();
       ditr++) {

    // If uncoveredRanges and uncoveredBranches don't exist, then the
    // symbol was never referenced by any executable.  Just skip it.
    if ((ditr->second.uncoveredRanges == NULL) &&
        (ditr->second.uncoveredBranches == NULL))
      continue;

    // If uncoveredRanges and uncoveredBranches are empty, then everything
    // must have been covered for this symbol.  Just skip it.
    if ((ditr->second.uncoveredRanges->set.empty()) &&
        (ditr->second.uncoveredBranches->set.empty()))
      continue;

    theCoverageMap = ditr->second.unifiedCoverageMap;
    bAddress = ditr->second.baseAddress;
    theInstructions = &(ditr->second.instructions);

    // Add annotations to each line where necessary
    for (itr = theInstructions->begin();
         itr != theInstructions->end();
         itr++ ) {

      std::string  annotation = "";
      std::string  line;
      char         textLine[100];

      state = A_SOURCE;

      if ( itr->isInstruction ) {
        if (!theCoverageMap->wasExecuted( itr->address - bAddress )){
          annotation = "<== NOT EXECUTED";
          state = A_NEVER_EXECUTED;
        } else if (theCoverageMap->isBranch( itr->address - bAddress )) {
          if (theCoverageMap->wasAlwaysTaken( itr->address - bAddress )){
            annotation = "<== ALWAYS TAKEN";
            state = A_BRANCH_TAKEN;
          } else if (theCoverageMap->wasNeverTaken( itr->address - bAddress )){
            annotation = "<== NEVER TAKEN";
            state = A_BRANCH_NOT_TAKEN;
          }
        } else {
          state = A_EXECUTED;
        }
      }

      sprintf( textLine, "%-70s", itr->line.c_str() );
      line = textLine + annotation;
      
      PutAnnotatedLine( aFile, state, line); 
    }
  }

  CloseAnnotatedFile( aFile );
}

/*
 *  Write branch report
 */
void ReportsBase::WriteBranchReport(
  const char* const fileName
) {
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  FILE*                                           report = NULL;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theBranches;
  unsigned int                                    count;

  // Open the branch report file
  report = OpenBranchFile( fileName );
  if (!report)
    return;

  // If no branches were found, then branch coverage is not supported
  if (SymbolsToAnalyze->getNumberBranchesFound() == 0)
    PutNoBranchInfo(report);

  // If branches were found, ...
  else {

    // Process uncovered branches for each symbol.
    count = 0;
    for (ditr = SymbolsToAnalyze->set.begin();
         ditr != SymbolsToAnalyze->set.end();
         ditr++) {

      theBranches = ditr->second.uncoveredBranches;

      if (theBranches && !theBranches->set.empty()) {

        for (ritr =  theBranches->set.begin() ;
             ritr != theBranches->set.end() ;
             ritr++ ) {
          count++;
          PutBranchEntry( report, count, ditr, ritr );
        }
      }
    }
  }

  CloseBranchFile( report );
}

/*
 *  Write coverage report
 */
void ReportsBase::WriteCoverageReport(
  const char* const fileName
) {
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  FILE*                                           report;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theRanges;
  unsigned int                                    count;

  // Open the coverage report file.
  report = OpenCoverageFile( fileName );
  if ( !report ) {
    return;
  }

  // Process uncovered ranges for each symbol.
  count = 0;
  for (ditr = SymbolsToAnalyze->set.begin();
       ditr != SymbolsToAnalyze->set.end();
       ditr++) {

    theRanges = ditr->second.uncoveredRanges;

    // If uncoveredRanges doesn't exist, then the symbol was never
    // referenced by any executable.  There may be a problem with the
    // desired symbols list or with the executables so put something
    // in the report.
    if (theRanges == NULL) {
      putCoverageNoRange( report, count, ditr->first );
      count++;
    }  else if (!theRanges->set.empty()) {

      for (ritr =  theRanges->set.begin() ;
           ritr != theRanges->set.end() ;
           ritr++ ) {
        PutCoverageLine( report, count, ditr, ritr );
        count++;
      }
    }
  }

  CloseCoverageFile( report );
}

/*
 * Write size report
 */
void ReportsBase::WriteSizeReport(
  const char* const fileName
) 
{
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  FILE*                                           report;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theRanges;
  unsigned int                                    count;

  // Open the report file.
  report = OpenSizeFile( fileName );
  if ( !report ) {
    return;
  }

  // Process uncovered ranges for each symbol.
  count = 0;
  for (ditr = SymbolsToAnalyze->set.begin();
       ditr != SymbolsToAnalyze->set.end();
       ditr++) {

    theRanges = ditr->second.uncoveredRanges;

    if (theRanges && !theRanges->set.empty()) {

      for (ritr =  theRanges->set.begin() ;
           ritr != theRanges->set.end() ;
           ritr++ ) {
        PutSizeLine( report, count, ditr, ritr );
        count++;
      }
    }
  }

  CloseSizeFile( report );
}

void GenerateReports()
{
  typedef std::list<ReportsBase *> reportList_t;

  reportList_t           reportList;
  reportList_t::iterator ritr;
  std::string            reportName;
  ReportsBase*           reports;

  reports = new ReportsText();
  reportList.push_back(reports);
  reports = new ReportsHtml();
  reportList.push_back(reports);

  for (ritr = reportList.begin(); ritr != reportList.end(); ritr++ ) {
    reports = *ritr;

    reportName = "annotated" + reports->ReportExtension();
    reports->WriteAnnotatedReport( reportName.c_str() );

    reportName = "branch" + reports->ReportExtension();
    reports->WriteBranchReport(reportName.c_str() );

    reportName = "uncovered" + reports->ReportExtension();
    reports->WriteCoverageReport(reportName.c_str() );

    reportName = "sizes" + reports->ReportExtension();
    reports->WriteSizeReport(reportName.c_str() );

  }

  for (ritr = reportList.begin(); ritr != reportList.end(); ritr++ ) {
    reports = *ritr;
    delete reports;
  }
  
}

}
