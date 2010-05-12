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

  void ReportsHtml::WriteIndex(
    const char* const fileName
  )
  {
    #define PRINT_ITEM( _t, _n ) \
       fprintf( \
         aFile, \
         "<li>%s (<a href=\"%s.html\">html</a> or "\
         "<a href=\"%s.txt\">text</a>)</li>\n", \
        _t, _n, _n );
    #define PRINT_TEXT_ITEM( _t, _n ) \
       fprintf( \
         aFile, \
         "<li>%s (<a href=\"%s\">text</a>)\n", \
        _t, _n );

    FILE*  aFile;
    
    // Open the file
    aFile = OpenFile( fileName );

    fprintf( aFile, "<strong>Reports Available</strong>\n" );
    fprintf( aFile, "<ul>\n" );

    PRINT_ITEM( "Coverage Report",      "uncovered" );
    PRINT_ITEM( "Branch Report",        "branch" );
    PRINT_ITEM( "Annotated Assembly",   "annotated" );
    PRINT_ITEM( "Symbol Summary",       "symbolSummary" );
    PRINT_ITEM( "Size Report",          "sizes" );

    PRINT_TEXT_ITEM( "Explanations Not Found", "ExplanationsNotFound.txt" );

    fprintf( aFile, "</li>\n" );

    CloseFile( aFile );

    #undef PRINT_ITEM
    #undef PRINT_TEXT_ITEM
  }

  FILE* ReportsHtml::OpenFile(
    const char* const fileName
  )
  {
    FILE*  aFile;
    
    // Open the file
    aFile = ReportsBase::OpenFile( fileName );

    // Put Header information on the file
    fprintf(
      aFile,
      "<html>\n"
      "<meta http-equiv=\"Content-Language\" content=\"English\" >\n"
      "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=us-ascii\" >\n"
      "<link rel=\"stylesheet\" type=\"text/css\" href=\"covoar.css\" media=\"screen\" >\n"
      "<script type=\"text/javascript\" src=\"table.js\"></script>\n"
      "<body>\n"
    );

    return aFile;
  }

  FILE* ReportsHtml::OpenAnnotatedFile(
    const char* const fileName
  )
  {
    FILE *aFile;

    // Open the file
    aFile = OpenFile(fileName);

    fprintf(
      aFile,
      "<pre class=\"code\">\n"
    );

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
      fprintf(
        aFile,
        "<table class=\"covoar table-autosort:0 table-autofilter table-autopage:10 table-page-number:pagenum table-page-count:pages\">\n"
        "<thead>\n"
        "<tr>\n"
        "<th class=\"table-sortable:default\" align=\"left\">Symbol</th>\n"
        "<th class=\"table-filterable table-sortable:default\" align=\"left\">Line</th>\n"
        "<th class=\"table-sortable:numeric\" align=\"left\">Size </br>Bytes</th>\n"
        "<th class=\"table-sortable:numeric\" align=\"left\">Size </br>Instructions</th>\n"
        "<th class=\"table-sortable:default\" align=\"left\">Reason</th>\n"
        "<th class=\"table-filterable table-sortable:default\" align=\"left\">Classification</th>\n"
        "<th class=\"table-sortable:default\" align=\"left\">Explanation</th>\n"
        "</tr>\n"
        "</thead>\n"
      );
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
    fprintf(
      aFile,
      "<table class=\"covoar table-autosort:0 table-autofilter table-autopage:10 table-page-number:pagenum table-page-count:pages\">\n"
      "<thead>\n"
      "<tr>\n"
      "<th class=\"table-sortable:default\" align=\"left\">Symbol</th>\n"
      "<th class=\"table-sortable:default\" align=\"left\">Range</th>\n"
      "<th class=\"table-sortable:numeric\" align=\"left\">Size </br>Bytes</th>\n"
      "<th class=\"table-sortable:numeric\" align=\"left\">Size </br>Instructions</th>\n"
      "<th class=\"table-filterable table-sortable:default\" align=\"left\">Classification</th>\n"
      "<th class=\"table-sortable:default\" align=\"left\">Explanation</th>\n"
      "</tr>\n"
      "</thead>\n"
     );

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
    fprintf(
      aFile,
      "<table class=\"covoar table-autosort:0 table-autofilter table-autopage:10 table-page-number:pagenum table-page-count:pages\">\n"
      "<thead>\n"
      "<tr>\n"
      "<th class=\"table-sortable:numeric\" align=\"left\">Size</th>\n"
      "<th class=\"table-sortable:default\" align=\"left\">Symbol</th>\n"
      "<th class=\"table-sortable:default\" align=\"left\">File</th>\n"
      "</tr>\n"
      "</thead>\n"
    );
    return aFile;
  }

  FILE*  ReportsHtml::OpenSymbolSummaryFile(
    const char* const fileName
  )
  {
    FILE *aFile;

    // Open the file
    aFile = OpenFile(fileName);

    // Put header information into the file
    fprintf(
      aFile,
      "<table class=\"covoar-table\">\n"
      "<tbody class=\"covoar-tbody\">\n"
      "<tr class=\"covoar-tr covoar-tr-first\">\n"
      "<th class=\"covoar-th\">Size</th>\n"
      "<th class=\"covoar-th\">Symbol</th>\n"
      "<th class=\"covoar-th\">File</th>\n"
      "</tr>\n"
    );
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

    // For all the characters in the line replace html reserved special
    // characters and output the line. Note that for a /pre block this
    // is only a '<' symbol.
    for (unsigned int i=0; i<line.size(); i++ ) {
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
    if ( ( count%2 ) == 0 )
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
    else
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

    // symbol
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
      symbolPtr->first.c_str()
    );

    // line
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\"><a href =\"annotated.html#range%d\">%s</td>\n",     
      rangePtr->id,
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

    // See if an explanation is available and write the Classification and
    // the Explination Columns.
    explanation = AllExplanations->lookupExplanation( rangePtr->lowSourceLine );
    if ( !explanation ) {
      // Write Classification
      fprintf( 
        report, 
        "<td class=\"covoar-td\" align=\"center\">NONE</td>\n"
        "<td class=\"covoar-td\" align=\"center\">No Explanation</td>\n"
      );
    } else {
      char explanationFile[48];
      sprintf( explanationFile, "explanation%d.html", rangePtr->id );
      fprintf( 
        report, 
        "<td class=\"covoar-td\" align=\"center\">%s</td>\n"
        "<td class=\"covoar-td\" align=\"center\">"
        "<a href=\"%s\">Explanation</a></td>\n",
        explanation->classification.c_str(),
        explanationFile
      );
      WriteExplationFile( explanationFile, explanation );
    }

    fprintf( report, "</tr>\n");

    return true;
  }

  bool ReportsHtml::WriteExplationFile(
    const char*                  fileName,
    const Coverage::Explanation* explanation
  )
  {
    FILE* report;

    report = OpenFile( fileName );

    for ( unsigned int i=0 ; i < explanation->explanation.size(); i++) {
      fprintf(
	report,
	"%s\n",
	explanation->explanation[i].c_str()
      );
    }
    CloseFile( report );
    return true;
  }

  void ReportsHtml::putCoverageNoRange(
    FILE*         report,
    unsigned int  count,
    std::string   symbol
  )
  {
    Coverage::Explanation explanation;

    explanation.explanation.push_back(
      "<html><p>\n"
      "This symbol was never referenced by an analyzed executable.  "
      "Therefore there is no size or disassembly for this symbol.  "
      "This could be due to symbol misspelling or lack of a test for "
      "this symbol."
      "</p></html>\n"
    );

    // Mark the background color different for odd and even lines.
    if ( ( count%2 ) == 0 )
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
    else
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

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
      "<td class=\"covoar-td\" align=\"center\">NONE</td>\n"
      "<td class=\"covoar-td\" align=\"center\">"
      "<a href=\"NotReferenced.html\">Explanation</a></td>\n"
    );
    WriteExplationFile( "NotReferenced.html", &explanation );

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
    if ( ( count%2 ) == 0 )
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-even\">\n");
    else
      fprintf( report, "<tr class=\"covoar-tr covoar-tr-odd\">\n");

    // symbol
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\">%s</td>\n",     
      symbolPtr->first.c_str()
    );

    // Range
    fprintf( 
      report, 
      "<td class=\"covoar-td\" align=\"center\"><a href =\"annotated.html#range%d\">%s </br>%s</td>\n",
      rangePtr->id,    
      rangePtr->lowSourceLine.c_str(),
      rangePtr->highSourceLine.c_str()
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
      char explanationFile[48];

      sprintf( explanationFile, "explanation%d.html", rangePtr->id );
      fprintf( 
        report, 
        "<td class=\"covoar-td\" align=\"center\">%s</td>\n"
        "<td class=\"covoar-td\" align=\"center\">"
        "<a href=\"%s\">Explanation</a></td>\n",
        explanation->classification.c_str(),
        explanationFile
      );
      WriteExplationFile( explanationFile, explanation );
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

    fprintf( report, "</tr>\n");

    return true;
  }

  bool  ReportsHtml::PutSymbolSummaryLine(
    FILE*                                           report,
    unsigned int                                    count,
    Coverage::DesiredSymbols::symbolSet_t::iterator symbol,
    Coverage::CoverageRanges::ranges_t::iterator    range
  )
  {
    // Mark the background color different for odd and even lines.
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

    fprintf( report, "</tr>\n");
    return true;
  }

  void ReportsHtml::CloseAnnotatedFile(
    FILE*  aFile
  )
  {
    fprintf(
      aFile,
      "</pre>\n"
      "</body>\n"
      "</html>"
    );

    CloseFile(aFile);
  }

  void ReportsHtml::CloseBranchFile(
    FILE*  aFile,
    bool   hasBranches
  )
  {
    if ( hasBranches ) {
      fprintf(
        aFile,
        "<tfoot>\n"
        "<tr>\n"
        "<td class=\"table-page:previous\" style=\"cursor:pointer;\">&lt; &lt; Previous</td>\n"
        "<td colspan=\"5\" style=\"text-align:center;\">Page <span id=\"pagenum\"></span>&nbsp;of <span id=\"pages\"></span></td>\n"
        "<td class=\"table-page:next\" style=\"cursor:pointer;\">Next &gt; &gt;</td>\n"
        "</tr>\n"
        "</tfoot>\n"
        "</tbody>\n"
        "</table>\n" 
      );
    }
    fprintf(
      aFile,
      "</pre>\n" 
      "</body>\n"
      "</html>"
    );

    CloseFile(aFile);
  }

  void ReportsHtml::CloseCoverageFile(
    FILE*  aFile
  )
  {
    fprintf(
      aFile,
      "<tfoot>\n"
      "<tr>\n"
      "<td class=\"table-page:previous\" style=\"cursor:pointer;\">&lt; &lt; Previous</td>\n"
      "<td colspan=\"4\" style=\"text-align:center;\">Page <span id=\"pagenum\"></span>&nbsp;of <span id=\"pages\"></span></td>\n"
      "<td class=\"table-page:next\" style=\"cursor:pointer;\">Next &gt; &gt;</td>\n"
      "</tr>\n"
      "</tfoot>\n"
      "</tbody>\n"
      "</table>\n" 
      "</pre>\n" 
      "</body>\n"
      "</html>"
    );

    CloseFile(aFile);
  }

  void ReportsHtml::CloseSizeFile(
    FILE*  aFile
  )
  {
    fprintf(
      aFile,
      "<tfoot>\n"
      "<tr>\n"
      "<td class=\"table-page:previous\" style=\"cursor:pointer;\">&lt; &lt; Previous</td>\n"
      "<td colspan=\"1\" style=\"text-align:center;\">Page <span id=\"pagenum\"></span>&nbsp;of <span id=\"pages\"></span></td>\n"
      "<td class=\"table-page:next\" style=\"cursor:pointer;\">Next &gt; &gt;</td>\n"
      "</tr>\n"
      "</tfoot>\n"
      "</tbody>\n"
      "</table>\n" 
      "</pre>\n" 
      "</body>\n"
      "</html>"
    );

    CloseFile( aFile );
  }

  void ReportsHtml::CloseSymbolSummaryFile(
    FILE*  aFile
  )
  {
    fprintf( aFile, "</tbody>\n" );
    fprintf( aFile, "</table>\n" );
    fprintf( aFile, "</pre>\n" );
    fprintf( aFile,"</body>\n");
    fprintf( aFile,"</html>");

    CloseFile( aFile );
  }

}
