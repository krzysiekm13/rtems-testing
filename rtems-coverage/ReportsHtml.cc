/*
 *   $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ReportsHtml.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "DesiredSymbols.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"


namespace Coverage {

ReportsHtml::ReportsHtml():
  ReportsBase()
{
  reportExtension_m = ".html";
}

ReportsHtml::~ReportsHtml()
{
}

FILE* ReportsHtml::OpenFile(
  const char* const fileName
)
{
  FILE*  aFile;
  
  // Open the file
  aFile = ReportsBase::OpenFile( fileName );

  // Put Header information on the file
  fprintf( aFile, "<html>\n");
  fprintf( aFile, "<meta http-equiv=\"Content-Language\" content=\"English\" >\n");
  fprintf( 
    aFile, 
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=us-ascii\" >\n"
  );
  fprintf( 
    aFile, 
    "<link rel=\"stylesheet\" type=\"text/css\" href=\"covoar.css\" media=\"screen\" >\n"
  );
  fprintf( aFile, "<body>\n");

  lastState_m = A_SOURCE;
  fprintf( aFile, "<pre class=\"code\">\n" );

  return aFile;
}

FILE* ReportsHtml::OpenBranchFile(
  const char* const fileName,
  bool              hasBranches
)
{
  FILE *aFile;

  // Open the file
  aFile = OpenFile(fileName);

  if ( hasBranches ) {
    // Put header information into the file
    fprintf( aFile, "<table class=\"covoar-table\">\n");
    fprintf( aFile, "<tbody class=\"covoar-tbody\">\n");
    fprintf( aFile, "<tr class=\"covoar-tr covoar-tr-first\">\n");
    fprintf( aFile, "<th class=\"covoar-th\">Index</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Symbol</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Line</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Size</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Size</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Reason</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Classification</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Explanation</th>\n");
    fprintf( aFile, "</tr>\n");

    fprintf( aFile, "<tr class=\"covoar-tr covoar-tr-first\">\n");
    fprintf( aFile, "<th class=\"covoar-th\"></th>\n");
    fprintf( aFile, "<th class=\"covoar-th\"></th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Bytes</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\">Instructions</th>\n");
    fprintf( aFile, "<th class=\"covoar-th\"></th>\n");
    fprintf( aFile, "<th class=\"covoar-th\"></th>\n");
    fprintf( aFile, "<th class=\"covoar-th\"></th>\n");
    fprintf( aFile, "</tr>\n");
  }
 
  return aFile;
}

FILE*  ReportsHtml::OpenCoverageFile(
  const char* const fileName
)
{
  FILE *aFile;

  // Open the file
  aFile = OpenFile(fileName);

  // Put header information into the file
  fprintf( aFile, "<table class=\"covoar-table\">\n");
  fprintf( aFile, "<tbody class=\"covoar-tbody\">\n");
  fprintf( aFile, "<tr class=\"covoar-tr covoar-tr-first\">\n");
  fprintf( aFile, "<th class=\"covoar-th\">Index</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Symbol</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Range</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Size</br>Bytes</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Size</br>Instructions</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Classification</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Explanation</th>\n");
  fprintf( aFile, "</tr>\n");

  return aFile;
}

FILE*  ReportsHtml::OpenSizeFile(
  const char* const fileName
)
{
  FILE *aFile;

  // Open the file
  aFile = OpenFile(fileName);

  // Put header information into the file
  fprintf( aFile, "<table class=\"covoar-table\">\n");
  fprintf( aFile, "<tbody class=\"covoar-tbody\">\n");
  fprintf( aFile, "<tr class=\"covoar-tr covoar-tr-first\">\n");
  fprintf( aFile, "<th class=\"covoar-th\">Size</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">Symbol</th>\n");
  fprintf( aFile, "<th class=\"covoar-th\">File</th>\n");
  fprintf( aFile, "</tr>\n");

  return aFile;
}

void ReportsHtml::PutAnnotatedLine( 
  FILE*                aFile, 
  AnnotatedLineState_t state, 
  std::string          line, 
  uint32_t             id 
)
{
  std::string stateText;
  char        number[10];

  
  sprintf(number,"%d", id);

  // Set the stateText based upon the current state.
  switch (state) {
    case  A_SOURCE:
      stateText = "</pre>\n<pre class=\"code\">\n";
      break;
    case  A_EXECUTED:
      stateText = "</pre>\n<pre class=\"codeExecuted\">\n";
      break;
    case  A_NEVER_EXECUTED:
      stateText = "</pre>\n";
      stateText += "<a name=\"range";
      stateText += number;
      stateText += "\"></a><pre class=\"codeNotExecuted\">\n";
      break;
    case  A_BRANCH_TAKEN:
      stateText = "</pre>\n";
      stateText += "<a name=\"range";
      stateText += number;
      stateText += "\"></a><pre class=\"codeAlwaysTaken\">\n";
      break;
    case  A_BRANCH_NOT_TAKEN:
      stateText = "</pre>\n";
      stateText += "<a name=\"range";
      stateText += number;
      stateText += "\"></a><pre class=\"codeNeverTaken\">\n";
      break;
    default:
      fprintf(stderr, "ERROR:  ReportsHtml::PutAnnotatedLine Unknown state\n");
      exit( -1 );
      break;
  }

  // If the state has not changed there is no need to change the text block
  // format.  If it has changed close out the old format and open up the
  // new format.
  if ( state != lastState_m ) {
    fprintf( aFile, stateText.c_str() );
    lastState_m = state;
  }

  // For all the characters in the line replace html reserved special characters
  // and output the line. Note that for a /pre block this is only a '<' symbol.
  for (unsigned int i=0; i<line.size(); i++ )
  {
    if ( line[i] == '<' )
      fprintf( aFile, "&lt;" );
    else
      fprintf( aFile, "%c", line[i] );
  }
  fprintf( aFile, "\n");
}

bool ReportsHtml::PutNoBranchInfo(
  FILE* report
)
{
  if ( BranchInfoAvailable )
    fprintf( report, "All branch paths taken.\n" );
  else
    fprintf( report, "No branch information found.\n" );
  return true;
}

bool ReportsHtml::PutBranchEntry(
  FILE*                                            report,
  unsigned int                                     count,
  Coverage::DesiredSymbols::symbolSet_t::iterator  symbolPtr,
  Coverage::CoverageRanges::ranges_t::iterator     rangePtr
)
{
  const Coverage::Explanation* explanation;


  // Mark the background color different for odd and even lines.
  fprintf( report, "</tr>\n");
  if ( ( count%2 ) == 0 )
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
  else
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

  // index
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"><a href =\"annotated.html#range%d\">%d</td>\n",
     rangePtr->id,
     rangePtr->id
   );

  // symbol
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    symbolPtr->first.c_str()
  );

  // line
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    rangePtr->lowSourceLine.c_str()
  );
  
  // Size in bytes
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%d</td>\n",
    rangePtr->highAddress - rangePtr->lowAddress + 1
  );

  // Size in instructions
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">XXX</td>\n"
  ); 

  // Reason Branch was uncovered
  if (rangePtr->reason ==
    Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_ALWAYS_TAKEN)
    fprintf( 
      report,
      "<td class=\"covoar-td\" align=\"center\">Always Taken</td>\n"
    );
  else if (rangePtr->reason ==
    Coverage::CoverageRanges::UNCOVERED_REASON_BRANCH_NEVER_TAKEN)
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">Never Taken</td>\n"
    );

  // See if an explanation is available
  explanation = AllExplanations->lookupExplanation( rangePtr->lowSourceLine );
  if ( !explanation ) {
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">NONE</td>\n"
    );
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">No Explanation</td>\n"
    );
  } else {
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">%s</td>\n",
      explanation->classification.c_str()
    );
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">FILL ME IN</td>\n"
    );
#if 0
    for ( unsigned int i=0 ;
          i < explanation->explanation.size();
          i++) {
      fprintf(
        report,
        "%s\n",
        explanation->explanation[i].c_str()
      );
    }
#endif
  }

  fprintf( report, "</tr>\n");

  return true;
}

void ReportsHtml::putCoverageNoRange(
  FILE*         report,
  unsigned int  count,
  std::string   symbol
)
{

  // Mark the background color different for odd and even lines.
  fprintf( report, "</tr>\n");
  if ( ( count%2 ) == 0 )
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
  else
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

  // index
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"></td>\n"
   );

  // symbol
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    symbol.c_str()
  );

  // starting line
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"></td>\n"
   );
   
  // Size in bytes
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"></td>\n"
  );

  // Size in instructions
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"></td>\n"
  ); 

  // See if an explanation is available
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"></td>\n"
  );
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">"
    "This symbol was never referenced by an analyzed executable.  "
    "Therefore there is no size or disassembly for this symbol.  "
    "This could be due to symbol misspelling or lack of a test for"
    "this symbol.</td>\n"
  );

  fprintf( report, "</tr>\n");
}

bool ReportsHtml::PutCoverageLine(
  FILE*                                            report,
  unsigned int                                     count,
  Coverage::DesiredSymbols::symbolSet_t::iterator  symbolPtr,
  Coverage::CoverageRanges::ranges_t::iterator     rangePtr
)
{
  const Coverage::Explanation*   explanation;


  // Mark the background color different for odd and even lines.
  fprintf( report, "</tr>\n");
  if ( ( count%2 ) == 0 )
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
  else
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

  // index
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\"><a href =\"annotated.html#range%d\">%d</td>\n",
     rangePtr->id,
     rangePtr->id
   );

  // symbol
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    symbolPtr->first.c_str()
  );

  // starting line
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s(0x%x)</br>%s(0x%x)</td>\n",
    rangePtr->lowSourceLine.c_str(),
    rangePtr->lowAddress,
    rangePtr->highSourceLine.c_str(),
    rangePtr->highAddress
   );
   
  // Size in bytes
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%d</td>\n",
    rangePtr->highAddress - rangePtr->lowAddress + 1
  );

  // Size in instructions
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">XXX</td>\n"
  ); 

  // See if an explanation is available
  explanation = AllExplanations->lookupExplanation( rangePtr->lowSourceLine );
  if ( !explanation ) {
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">NONE</td>\n"
    );
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">No Explanation</td>\n"
    );
  } else {
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">%s</td>\n",
      explanation->classification.c_str()
    );
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">FILL ME IN</td>\n"
    );


    #if  0
    for ( unsigned int i=0; i < explanation->explanation.size(); i++) {
      fprintf( report,"%s\n", explanation->explanation[i].c_str() );
    }
    #endif
  }

  fprintf( report, "</tr>\n");

  return true;
}

bool  ReportsHtml::PutSizeLine(
  FILE*                                           report,
  unsigned int                                    count,
  Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
  Coverage::CoverageRanges::ranges_t::iterator    range
)
{
  // Mark the background color different for odd and even lines.
  fprintf( report, "</tr>\n");
  if ( ( count%2 ) == 0 )
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
  else
    fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

  // size
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%d</td>\n",
    range->highAddress - range->lowAddress + 1
  );

  // symbol
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    symbol->first.c_str()
  );

  // file
  fprintf( 
    report, 
    "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
    range->lowSourceLine.c_str()
  );



  return true;
}

void ReportsHtml::CloseAnnotatedFile(
  FILE*  aFile
)
{
  fprintf( aFile,"</pre>\n");
  fprintf( aFile,"</body>\n");
  fprintf( aFile,"</html>");

  CloseFile(aFile);
}

void ReportsHtml::CloseBranchFile(
  FILE*  aFile,
  bool   hasBranches
)
{
  if ( hasBranches ) {
    fprintf( aFile, "</tbody>\n" );
    fprintf( aFile, "</table>\n" );
  }
  fprintf( aFile, "</pre>\n" );
  fprintf( aFile, "</body>\n");
  fprintf( aFile, "</html>");

  CloseFile(aFile);
}

void ReportsHtml::CloseCoverageFile(
  FILE*  aFile
)
{
  fprintf( aFile, "</tbody>\n" );
  fprintf( aFile, "</table>\n" );
  fprintf( aFile, "</pre>\n" );
  fprintf( aFile,"</body>\n");
  fprintf( aFile,"</html>");

  CloseFile(aFile);
}

void ReportsHtml::CloseSizeFile(
  FILE*  aFile
)
{
  fprintf( aFile, "</tbody>\n" );
  fprintf( aFile, "</table>\n" );
  fprintf( aFile, "</pre>\n" );
  fprintf( aFile, "</pre>\n" );
  fprintf( aFile,"</body>\n");
  fprintf( aFile,"</html>");

  CloseFile( aFile );
}

}
