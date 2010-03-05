/*
 *  $Id$
 */

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <list>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "app_common.h"
#include "CoverageFactory.h"
#include "CoverageMap.h"
#include "DesiredSymbols.h"
#include "ExecutableInfo.h"
#include "Explanations.h"
#include "ObjdumpProcessor.h"
#include "Reports.h"

/*
 *  Variables to control general behavior
 */
const char*                          branchReportFile = "branch.txt";
char*                                coverageFileExtension = NULL;
std::list<std::string>               coverageFileNames;
Coverage::CoverageFormats_t          coverageFormat;
Coverage::CoverageReaderBase*        coverageReader = NULL;
const char*                          coverageReportFile = "uncovered.txt";
char*                                executable = NULL;
char*                                executableExtension = NULL;
int                                  executableExtensionLength = 0;
std::list<Coverage::ExecutableInfo*> executablesToAnalyze;
char*                                explanations = NULL;
char*                                noExplanations = NULL;
Coverage::ObjdumpProcessor*          objdumpProcessor = NULL;
char*                                progname;
bool                                 singleExecutable = false;
const char*                          sizeReportFile = "sizes.txt";
const char*                          symbolsFile = "rtems.syms";
char*                                target = NULL;

/*
 *  Global variables for the program
 */
Coverage::Explanations*   AllExplanations = NULL;
Coverage::DesiredSymbols* SymbolsToAnalyze = NULL;
Coverage::Toolnames*      Tools = NULL;

bool Verbose = false;


/*
 *  Print program usage message
 */
void usage()
{
  fprintf(
    stderr,
    "Usage: %s [-v] -T TARGET -f FORMAT [-E EXPLANATIONS] -1 EXECUTABLE coverage1 ... coverageN\n"
    "--OR--\n"
    "Usage: %s [-v] -T TARGET -f FORMAT [-E EXPLANATIONS] -e EXE_EXTENSION -c COVERAGEFILE_EXTENSION EXECUTABLE1 ... EXECUTABLE2\n"
    "\n"
    "  -v                        - verbose at initialization\n"
    "  -T TARGET                 - target name\n"
    "  -f FORMAT                 - coverage file format "
           "(RTEMS, QEMU, TSIM or Skyeye)\n"
    "  -E EXPLANATIONS           - name of file with explanations\n"
    "  -1 EXECUTABLE             - name of executable to get symbols from\n"
    "  -e EXE_EXTENSION          - extension of the executables to analyze\n"
    "  -c COVERAGEFILE_EXTENSION - extension of the coverage files to analyze\n"
    "\n",
    progname,
    progname
  );
}

#define PrintableString(_s) \
       ((!(_s)) ? "NOT SET" : (_s))

int main(
  int    argc,
  char** argv
)
{
  std::list<std::string>::iterator               citr;
  std::string                                    coverageFileName;
  std::list<Coverage::ExecutableInfo*>::iterator eitr;
  Coverage::ExecutableInfo*                      executableInfo = NULL;
  char*                                          format = NULL;
  int                                            i;
  int                                            opt;

  //
  // Process command line options.
  //
  progname = argv[0];

  while ((opt = getopt(argc, argv, "1:e:c:E:f:T:v")) != -1) {
    switch (opt) {
      case '1':
        singleExecutable = true;
        executableInfo = new Coverage::ExecutableInfo( optarg );
        executablesToAnalyze.push_back( executableInfo );
        break;
      case 'e':
        executableExtension = optarg;
        executableExtensionLength = strlen( executableExtension );
        break;
      case 'c': coverageFileExtension = optarg;  break;
      case 'E': explanations          = optarg;  break;
      case 'T': target                = optarg;  break;
      case 'v': Verbose               = true;    break;
      case 'f':
        format = optarg;
        coverageFormat = Coverage::CoverageFormatToEnum(optarg);
        break;
      default: /* '?' */
        usage();
        exit( -1 );
    }
  }

  // If a single executable was specified, process the remaining
  // arguments as coverage file names.
  if (singleExecutable) {
    for (i=optind; i < argc; i++) {
      coverageFileName = argv[i];
      coverageFileNames.push_back( coverageFileName );
    }
  }

  // If not envoked with a single executable, process the remaining
  // arguments as executables and derive the coverage file names.
  else {
    for (i = optind; i < argc; i++) {
      executableInfo = new Coverage::ExecutableInfo( argv[i] );
      executablesToAnalyze.push_back( executableInfo );
      coverageFileName = executableInfo->getFileName();
      coverageFileName.replace(
        coverageFileName.length() - executableExtensionLength,
        executableExtensionLength,
        coverageFileExtension
      );
      coverageFileNames.push_back( coverageFileName );
    }
  }

  if (Verbose) {
    if (singleExecutable)
      fprintf(
        stderr,
        "Processing a single executable and multiple coverage files\n"
      );
    else
      fprintf(
        stderr,
        "Processing multiple executable/coverage file pairs\n"
      );
    fprintf( stderr, "Coverage Format : %s\n", format );
    fprintf( stderr, "Target          : %s\n", PrintableString(target) );
    fprintf( stderr, "\n" );
  }

  //
  // Validate inputs.
  //

  // Target name must be set.
  if (!target) {
    fprintf( stderr, "ERROR: target not specified\n" );
    usage();
    exit(-1);
  }

  // Validate format.
  if (!format) {
    fprintf( stderr, "ERROR: coverage format report not specified\n" );
    usage();
    exit(-1);
  }

  //
  // Create data to support analysis.
  //

  // Create toolnames based on target.
  Tools = new Coverage::Toolnames( target );

  // Create the set of desired symbols.
  SymbolsToAnalyze = new Coverage::DesiredSymbols();
  SymbolsToAnalyze->load( symbolsFile );
  if (Verbose)
    fprintf(
      stderr, "Analyzing %d symbols\n", SymbolsToAnalyze->set.size()
    );

  // Create explanations.
  AllExplanations = new Coverage::Explanations();
  AllExplanations->load( explanations );

  // Create coverage map reader.
  coverageReader = Coverage::CreateCoverageReader(coverageFormat);
  if (!coverageReader) {
    fprintf( stderr, "ERROR: Unable to create coverage file reader\n" );
    exit(-1);
  }

  // Create the objdump processor.
  objdumpProcessor = new Coverage::ObjdumpProcessor();

  // Process each executable to analyze.
  for (eitr = executablesToAnalyze.begin();
       eitr != executablesToAnalyze.end();
       eitr++) {

    if (Verbose)
      fprintf(
        stderr,
        "Extracting information from %s\n",
        ((*eitr)->getFileName()).c_str()
      );

    // Create the executable information.
    (*eitr)->initialize();

    // Load the objdump for the symbols in this executable.
    objdumpProcessor->load( *eitr );
  }

  //
  // Analyze the coverage data.
  //

  // Process each executable/coverage file pair.
  eitr = executablesToAnalyze.begin();
  for (citr = coverageFileNames.begin();
       citr != coverageFileNames.end();
       citr++) {

    if (Verbose)
      fprintf(
        stderr,
        "Processing coverage file %s for executable %s\n",
        (*citr).c_str(),
        ((*eitr)->getFileName()).c_str()
    );

    // along with its coverage file.
    if (coverageReader->processFile( (*citr).c_str(), *eitr )) {

      // Merge each symbols coverage map into a unified coverage map.
      (*eitr)->mergeCoverage();
    }

    if (!singleExecutable)
      eitr++;
  }

  // Determine the uncovered ranges and branches.
  if (Verbose)
    fprintf( stderr, "Computing uncovered ranges and branches\n" );
  SymbolsToAnalyze->computeUncovered();

  // Look up the source lines for any uncovered ranges and branches.
  if (Verbose)
    fprintf(
      stderr, "Looking up source lines for uncovered ranges and branches\n"
    );
  SymbolsToAnalyze->findSourceForUncovered();

  //
  // Report the coverage data.
  //

  // Generate report of ranges not executed.
  if (Verbose)
    fprintf( stderr, "Writing coverage report (%s)\n", coverageReportFile );
  Coverage::WriteCoverageReport( coverageReportFile );

  // Generate report of branches taken/not taken.
  if (Verbose)
    fprintf( stderr, "Writing branch report (%s)\n", branchReportFile );
  Coverage::WriteBranchReport( branchReportFile );

  // Generate report of range sizes.
  if (Verbose)
    fprintf( stderr, "Writing size report (%s)\n", sizeReportFile );
  Coverage::WriteSizeReport( sizeReportFile );

  // Generate annotated assembly file
  if (Verbose)
    fprintf( stderr, "Writing annotated report (%s)\n", "annotated.txt" );
  Coverage::WriteAnnotatedReport( "annotated.txt" );

  // Write explanations that were not found.
  std::string str = explanations;
  str = str + ".NotFound";
  if (Verbose)
    fprintf( stderr, "Writing Not Found Report (%s)\n", str.c_str() );
  AllExplanations->writeNotFound(str.c_str());

  // Calculate coverage statistics and output results.
  {
    uint32_t                                        a;
    uint32_t                                        endAddress;
    Coverage::DesiredSymbols::symbolSet_t::iterator itr;
    uint32_t                                        notExecuted = 0;
    double                                          percentage;
    Coverage::CoverageMapBase*                      theCoverageMap;
    uint32_t                                        totalBytes = 0;

    // Look at each symbol.
    for (itr = SymbolsToAnalyze->set.begin();
         itr != SymbolsToAnalyze->set.end();
         itr++) {

      // If the symbol's unified coverage map exists, scan through it
      // and count bytes.
      theCoverageMap = itr->second.unifiedCoverageMap;
      if (theCoverageMap) {

        endAddress = itr->second.size - 1;

        for (a = 0; a < endAddress; a++) {
          totalBytes++;
          if (!theCoverageMap->wasExecuted( a ))
            notExecuted++;
        }
      }
    }

    percentage = (double) notExecuted;
    percentage /= (double) totalBytes;
    percentage *= 100.0;

    printf( "Bytes Analyzed           : %d\n", totalBytes );
    printf( "Bytes Not Executed       : %d\n", notExecuted );
    printf( "Percentage Executed      : %5.4g\n", 100.0 - percentage  );
    printf( "Percentage Not Executed  : %5.4g\n", percentage  );
    printf(
      "Uncovered ranges found   : %d\n",
      SymbolsToAnalyze->getNumberUncoveredRanges()
    );
    if (SymbolsToAnalyze->getNumberBranchesFound() == 0)
      printf( "No branch information found\n" );
    else {
      printf(
        "Total branches found     : %d\n",
        SymbolsToAnalyze->getNumberBranchesFound()
      );
      printf(
        "Uncovered branches found : %d\n",
        SymbolsToAnalyze->getNumberBranchesAlwaysTaken() +
         SymbolsToAnalyze->getNumberBranchesNeverTaken()
      );
      printf(
        "   %d branches always taken\n",
        SymbolsToAnalyze->getNumberBranchesAlwaysTaken()
      );
      printf(
        "   %d branches never taken\n",
        SymbolsToAnalyze->getNumberBranchesNeverTaken()
      );
    }
  }

  return 0;
}
