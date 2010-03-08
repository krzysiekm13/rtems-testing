/*
 *   $Id$
 */

#include <stdio.h>
#include <string.h>

#include "Reports.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "DesiredSymbols.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"

/*
 *  Write annotated report
 */
void Coverage::WriteAnnotatedReport(
  const char* const fileName
) {
  FILE*                                           aText = NULL;
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  std::list<Coverage::ObjdumpProcessor::objdumpLine_t>* theInstructions;
  std::list<Coverage::ObjdumpProcessor::objdumpLine_t>::iterator itr;
  Coverage::CoverageMapBase*                      theCoverageMap = NULL;
  uint32_t                                        bAddress = 0;

  // Open the annotated report file.
  aText = fopen( fileName, "w" );
  if ( !aText ) {
    fprintf(
      stderr, "Unable to open %s\n", fileName
    );
    return;
  }

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

      const char *annotation = NULL;

      if ( itr->isInstruction ) {
        if (!theCoverageMap->wasExecuted( itr->address - bAddress ))
          annotation = "\t<== NOT EXECUTED";
        else if (theCoverageMap->isBranch( itr->address - bAddress )) {
          if (theCoverageMap->wasAlwaysTaken( itr->address - bAddress ))
            annotation = "\t<== ALWAYS TAKEN";
          else if (theCoverageMap->wasNeverTaken( itr->address - bAddress ))
            annotation = "\t<== NEVER TAKEN";
        }
      }

      if ( !annotation )
        fprintf( aText, "%s\n", itr->line.c_str() );
      else
        fprintf( aText, "%-76s%s\n", itr->line.c_str(), annotation );
    }
  }

  fclose( aText );
}

/*
 *  Write branch report
 */
void Coverage::WriteBranchReport(
  const char* const fileName
) {
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  const Coverage::Explanation*                    explanation;
  FILE*                                           report = NULL;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theBranches;

  // Open the branch report file
  report = fopen( fileName, "w" );
  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  // If no branches were found, then branch coverage is not supported
  if (SymbolsToAnalyze->getNumberBranchesFound() == 0)
    fprintf( report, "No branch information found\n" );

  // If branches were found, ...
  else {

    // Process uncovered branches for each symbol.
    for (ditr = SymbolsToAnalyze->set.begin();
         ditr != SymbolsToAnalyze->set.end();
         ditr++) {

      theBranches = ditr->second.uncoveredBranches;

      if (theBranches && !theBranches->set.empty()) {

        for (ritr =  theBranches->set.begin() ;
             ritr != theBranches->set.end() ;
             ritr++ ) {

          // Add an entry to the report
          fprintf(
            report,
            "============================================\n"
            "Symbol        : %s (0x%x)\n"
            "Line          : %s (0x%x)\n"
            "Size in Bytes : %d\n",
            ditr->first.c_str(),
            ditr->second.baseAddress,
            ritr->lowSourceLine.c_str(),
            ritr->lowAddress,
            ritr->highAddress - ritr->lowAddress + 1
          );

          if (ritr->reason ==
           Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_ALWAYS_TAKEN)
            fprintf(
              report, "Reason        : %s\n\n", "ALWAYS TAKEN"
            );
          else if (ritr->reason ==
           Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_NEVER_TAKEN)
            fprintf(
              report, "Reason        : %s\n\n", "NEVER TAKEN"
            );

          // See if an explanation is available
          explanation =
           AllExplanations->lookupExplanation( ritr->lowSourceLine );

          if ( !explanation ) {
            fprintf(
              report,
              "Classification: NONE\n"
              "\n"
              "Explanation:\n"
              "No Explanation\n"
            );
          } else {
            fprintf(
              report,
              "Classification: %s\n"
              "\n"
              "Explanation:\n",
              explanation->classification.c_str()
            );

            for ( unsigned int i=0 ;
                  i < explanation->explanation.size();
                  i++) {
              fprintf(
                report,
                "%s\n",
                explanation->explanation[i].c_str()
              );
            }
          }

          fprintf(
            report, "============================================\n"
          );
        }
      }
    }
  }

  fclose( report );
}

/*
 *  Write coverage report
 */
void Coverage::WriteCoverageReport(
  const char* const fileName
) {
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  const Coverage::Explanation*                    explanation;
  FILE*                                           report;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theRanges;

  // Open the coverage report file.
  report = fopen( fileName, "w" );
  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  // Process uncovered ranges for each symbol.
  for (ditr = SymbolsToAnalyze->set.begin();
       ditr != SymbolsToAnalyze->set.end();
       ditr++) {

    theRanges = ditr->second.uncoveredRanges;

    // If uncoveredRanges doesn't exist, then the symbol was never
    // referenced by any executable.  There may be a problem with the
    // desired symbols list or with the executables so put something
    // in the report.
    if (theRanges == NULL) {

      fprintf(
        report,
        "============================================\n"
        "Symbol        : %s\n\n"
        "          *** NEVER REFERENCED ***\n\n"
        "This symbol was never referenced by an analyzed executable.\n"
        "Therefore there is no size or disassembly for this symbol.\n"
        "This could be due to symbol misspelling or lack of a test for\n"
        "this symbol.\n"
        "============================================\n",
        ditr->first.c_str()
      );
    }

    else if (!theRanges->set.empty()) {

      for (ritr =  theRanges->set.begin() ;
           ritr != theRanges->set.end() ;
           ritr++ ) {

        fprintf(
          report,
          "============================================\n"
          "Symbol        : %s (0x%x)\n"
          "Starting Line : %s (0x%x)\n"
          "Ending Line   : %s (0x%x)\n"
          "Size in Bytes : %d\n\n",
          ditr->first.c_str(),
          ditr->second.baseAddress,
          ritr->lowSourceLine.c_str(),
          ritr->lowAddress,
          ritr->highSourceLine.c_str(),
          ritr->highAddress,
          ritr->highAddress - ritr->lowAddress + 1
        );

        explanation =
         AllExplanations->lookupExplanation( ritr->lowSourceLine );

        if ( !explanation ) {
          fprintf(
            report,
            "Classification: NONE\n"
            "\n"
            "Explanation:\n"
            "No Explanation\n"
          );
        } else {
          fprintf(
            report,
            "Classification: %s\n"
            "\n"
            "Explanation:\n",
            explanation->classification.c_str()
          );

          for ( unsigned int i=0 ;
                i < explanation->explanation.size();
                i++) {
            fprintf(
              report,
              "%s\n",
              explanation->explanation[i].c_str()
            );
          }
        }

        fprintf(
          report,
          "============================================\n"
        );
      }
    }
  }

  fclose( report );
}

/*
 * Write size report
 */
void Coverage::WriteSizeReport(
  const char* const fileName
) {
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr;
  FILE*                                           report;
  Coverage::CoverageRanges::ranges_t::iterator    ritr;
  Coverage::CoverageRanges*                       theRanges;

  // Open the report file.
  report = fopen( fileName, "w" );
  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  // Process uncovered ranges for each symbol.
  for (ditr = SymbolsToAnalyze->set.begin();
       ditr != SymbolsToAnalyze->set.end();
       ditr++) {

    theRanges = ditr->second.uncoveredRanges;

    if (theRanges && !theRanges->set.empty()) {

      for (ritr =  theRanges->set.begin() ;
           ritr != theRanges->set.end() ;
           ritr++ ) {
        fprintf(
          report,
          "%d\t%s\t%s\n",
          ritr->highAddress - ritr->lowAddress + 1,
          ditr->first.c_str(),
          ritr->lowSourceLine.c_str()
        );
      }
    }
  }

  fclose( report );
}
