/*
 *   $Id$
 */

#include <stdio.h>
#include <string.h>

#include "Reports.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"

/*
 *  Write annotated report
 */
void WriteAnnotatedReport(
  const char *fileName,
  uint32_t    lowAddress,
  uint32_t    highAddress
) {
  FILE *aText = NULL;
  std::list<Coverage::ObjdumpLine>::iterator it;

  // Open Annotated Text File
  aText = fopen( fileName, "w" );
  if ( !aText ) {
    fprintf(
      stderr,
      "ObjdumpProcessor::writeAnnotated - unable to open %s\n",
      fileName
    );
    return;
  }

  // Add annotations to each line where necessary
  for (it = ObjdumpProcessor->Contents.begin();
       it != ObjdumpProcessor->Contents.end();
       it++ ) {

    const char *annotation = NULL;

    if ( it->isInstruction &&
         it->address >= lowAddress && it->address <= highAddress )  {

      if ( !CoverageMap->wasExecuted( it->address ) )
        annotation = "\t<== NOT EXECUTED";
      else if ( CoverageMap->isBranch( it->address ) ) {
        if ( CoverageMap->wasAlwaysTaken( it->address ) )
          annotation = "\t<== ALWAYS TAKEN";
        else if ( CoverageMap->wasNeverTaken( it->address ) )
          annotation = "\t<== NEVER TAKEN";
      }

    }
    if ( !annotation )
      fprintf( aText, "%s\n", it->line.c_str() );
    else
      fprintf( aText, "%-76s%s\n", it->line.c_str(), annotation );
  }

  fclose( aText );
}

/*
 *  Write branch report
 */
void WriteBranchReport(
  const char *fileName,
  uint32_t    lowAddress,
  uint32_t    highAddress
) {
  uint32_t                     address;
  std::string                  branchLine;
  const Coverage::Explanation *explanation;
  FILE                        *report = NULL;

  // Open the branch report file
  report = fopen( fileName, "w" );
  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  // If no branches were found, then branch coverage is not supported
  if (!BranchesFound)
    fprintf( report, "No branch information found\n" );

  // If branches were found, then check each branch to determine if
  // it was always taken or never taken
  else {
    for ( address = lowAddress; address < highAddress; address++ ) {

      if (CoverageMap->isBranch( address ) &&
          (CoverageMap->wasAlwaysTaken( address ) ||
           CoverageMap->wasNeverTaken( address ))) {

        branchLine  = CoverageMap->getSourceLine( address );

        // Add an entry to the report
        fprintf(
          report,
          "============ Branch: %08x ==================\n"
          "%08x : %s\n",
          address,
          address, branchLine.c_str()
        );

        if (CoverageMap->wasAlwaysTaken( address ))
          fprintf(
            report, "Status : %s\n\n", "ALWAYS TAKEN"
          );
        else if (CoverageMap->wasNeverTaken( address ))
          fprintf(
            report, "Status : %s\n\n", "NEVER TAKEN"
          );

        // See if an explanation is available
        explanation = Explanations->lookupExplanation( branchLine );

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
          report, "====================================================\n"
        );
      }
    }
  }

  fclose( report );
}

/*
 *  Write coverage report
 */
void WriteCoverageReport(
  const char *fileName
) {
  FILE *report;
  std::list<Coverage::CoverageRange>::iterator it;

  /*
   *  Now begin to write the real report
   */
  report = fopen( fileName, "w" );

  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  for (it =  Ranges->Set.begin() ;
       it != Ranges->Set.end() ;
       it++ ) {
    const Coverage::Explanation *explanation;
    std::string lowLine;
    std::string highLine;

    lowLine  = CoverageMap->getSourceLine( it->lowAddress );
    highLine = CoverageMap->getSourceLine( it->highAddress );

    explanation = Explanations->lookupExplanation( lowLine );

    fprintf(
      report,
      "============ Range: %08x - %08x ============\n"
      "%08x : %s\n"
      "%08x : %s\n"
      "Size : %d\n"
      "\n",
      it->lowAddress,   it->highAddress,
      it->lowAddress,   lowLine.c_str(),
      it->highAddress,  highLine.c_str(),
      it->highAddress - it->lowAddress + 1
    );

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
      "====================================================\n"
    );
  }

  fclose( report );
}

/*
 * Write size report
 */
void WriteSizeReport(
  const char *fileName
) {
  FILE *report;
  std::list<Coverage::CoverageRange>::iterator it;

  /*
   *  Now begin to write the real report
   */
  report = fopen( fileName, "w" );

  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", fileName );
    return;
  }

  for (it =  Ranges->Set.begin() ;
       it != Ranges->Set.end() ;
       it++ ) {
    fprintf(
      report,
      "%s\t%d\n",
      CoverageMap->getSourceLine( it->lowAddress ).c_str(),
      it->highAddress - it->lowAddress + 1
    );
  }

  fclose( report );
}
