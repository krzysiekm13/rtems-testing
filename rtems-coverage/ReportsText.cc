/*
 *   $Id$
 */

#include <stdio.h>
#include <string.h>

#include "ReportsText.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "DesiredSymbols.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"


namespace Coverage {

ReportsText::ReportsText():
  ReportsBase()
{
  reportExtension_m = ".txt";
}

void ReportsText::PutAnnotatedLine( 
  FILE*                aFile, 
  AnnotatedLineState_t state, 
  std::string          line 
)
{
  fprintf( aFile, "%s\n", line.c_str());
}

bool ReportsText::PutNoBranchInfo(
  FILE*           report
)
{
  fprintf( report, "No branch information found\n" );
  return true;
}


bool ReportsText::PutBranchEntry(
  FILE*   report,
  unsigned int                                     number,
  Coverage::DesiredSymbols::symbolSet_t::iterator  symbolPtr,
  Coverage::CoverageRanges::ranges_t::iterator     rangePtr
)
{
  const Coverage::Explanation* explanation;

  // Add an entry to the report
  fprintf(
    report,
    "============================================\n"
    "Symbol        : %s (0x%x)\n"
    "Line          : %s (0x%x)\n"
    "Size in Bytes : %d\n",
    symbolPtr->first.c_str(),
    symbolPtr->second.baseAddress,
    rangePtr->lowSourceLine.c_str(),
    rangePtr->lowAddress,
    rangePtr->highAddress - rangePtr->lowAddress + 1
  );

  if (rangePtr->reason ==
    Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_ALWAYS_TAKEN)
    fprintf(
      report, "Reason        : %s\n\n", "ALWAYS TAKEN"
    );
  else if (rangePtr->reason ==
    Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_NEVER_TAKEN)
    fprintf( report, "Reason        : %s\n\n", "NEVER TAKEN" );

  // See if an explanation is available
  explanation = AllExplanations->lookupExplanation( rangePtr->lowSourceLine );

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

  return true;
}

void ReportsText::putCoverageNoRange(
  FILE*         report,
  unsigned int  number,
  std::string   symbol
)
{
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
        symbol.c_str()
      );
}

bool ReportsText::PutCoverageLine(
  FILE*                                       report,
  unsigned int                                     number,
  Coverage::DesiredSymbols::symbolSet_t::iterator ditr,
  Coverage::CoverageRanges::ranges_t::iterator    ritr
)
{
  const Coverage::Explanation*   explanation;

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

  explanation = AllExplanations->lookupExplanation( ritr->lowSourceLine );

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

    for ( unsigned int i=0; i < explanation->explanation.size(); i++) {
      fprintf( report,"%s\n", explanation->explanation[i].c_str() );
    }
  }

  fprintf(report, "============================================\n");
  return true;
}

bool  ReportsText::PutSizeLine(
  FILE*                                      report,
  unsigned int                                     number,
  Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
  Coverage::CoverageRanges::ranges_t::iterator    range
)
{
  fprintf(
    report,
    "%d\t%s\t%s\n",
    range->highAddress - range->lowAddress + 1,
    symbol->first.c_str(),
    range->lowSourceLine.c_str()
  );
  return true;
}



}
