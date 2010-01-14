/*
 *  $Id$
 */

#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>

#include "CoverageMap.h"
#include "CoverageFactory.h"
#include "ObjdumpProcessor.h"
#include "app_common.h"
#include "CoverageRanges.h"
#include "Explanations.h"

/*
 *  Variables to control global behavior
 */
int verbose = 0;
Coverage::CoverageFormats_t coverageFormat;
char *mergedCoverageFile = NULL;
char *coverageReportFile = NULL;
char *sizeReportFile = NULL;
uint32_t lowAddress  = 0xffffffff;
uint32_t highAddress = 0xffffffff;

char *target = NULL;
char *executable = NULL;
char *explanations = NULL;
char *noExplanations = NULL;
char *progname;

/*
 *  Global variables for the program
 */
Coverage::CoverageMapBase    *CoverageMap      = NULL;
Coverage::CoverageReaderBase *CoverageReader   = NULL;
Coverage::CoverageWriterBase *CoverageWriter   = NULL;
Coverage::Toolnames          *Tools            = NULL;
Coverage::ObjdumpProcessor   *ObjdumpProcessor = NULL;
Coverage::CoverageRanges     *Ranges           = NULL;
Coverage::Explanations       *Explanations     = NULL;

/*
 *  Set of addresses we need source line number for
 */
std::list<uint32_t> AddressesNeedingSourceLine; 

/*
 *  Convert string to int with status out
 */

bool stringToUint32(
  const char  *s,
  int          base,
  uint32_t    *n
)
{
  long long result;

  if ( !n )
    return false;

  errno = 0;
  *n    = 0;

  result = strtoll( s, NULL, base );

  if ( (result == 0) && errno )
    return false; 

  if ( (result == LLONG_MAX) && (errno == ERANGE))
    return false; 

  if ( (result == LLONG_MIN) && (errno == ERANGE))
    return false; 

  *n = (uint32_t)result;
  return true;
}

/*
 *  Print program usage message
 */
void usage()
{
  fprintf(
    stderr,
    "Usage: %s [-v] [-t] [-m file] -T TARGET [-e EXECUTABLE]-l ADDRESS -h ADDRESS coverage1... coverageN\n"
    "\n"
    "  -l low address   - low address of range to merge\n"
    "  -l high address  - high address of range to merge\n"
    "  -f format        - coverage files are in <format> "
                     "(RTEMS, TSIM or Skyeye)\n"
    "  -m FILE          - optional merged coverage file to write\n"
    "  -r REPORT        - optional coverage report to write\n"
    "  -s REPORT        - optional size report to write\n"
    "  -T TARGET        - target name\n"
    "  -e EXECUTABLE    - name of executable to get symbols from\n"
    "  -E EXPLANATIONS  - name of file with explanations\n"
    "  -v               - verbose at initialization\n"
    "\n",
    progname
  );
}

int UncoveredCases = 0;

/*
 *  Look over the coverage map and compute uncovered ranges
 */
void ComputeUncoveredRanges(void)
{
  uint32_t a, la, ha;
  std::list<Coverage::CoverageRange>::iterator it;

  /*
   *  Find all the unexecuted addresses and add them to the range.
   */
  for ( a=lowAddress ; a < highAddress ; a++ ) {
    if ( !CoverageMap->wasExecuted( a ) ) {
      la = a;
      for (ha=a+1 ; ha<=highAddress && !CoverageMap->wasExecuted(ha) ; ha++ )
        ;

      UncoveredCases++;
      Ranges->add( la, ha-1 );
      fprintf( stderr, "Push back %x %x\n", la, ha -1 );
      AddressesNeedingSourceLine.push_back( la ); 
      AddressesNeedingSourceLine.push_back( ha-1 ); 
      a = ha;
    }
  }
}

/*
 *  Find source lines for addresses
 */
void FindSourceForAddresses(void)
{
  FILE                          *tmpfile;
  std::list<uint32_t>::iterator  it; 

  /*
   *  Write a temporary file with ranges
   */
  if ( verbose )
    fprintf( stderr, "Writing ranges.tmp input to addr2line\n" );

  tmpfile = fopen( "ranges.tmp", "w" );
  if ( !tmpfile ) {
    fprintf( stderr, "Unable to open %s\n\n", "ranges.tmp" );
    exit(-1);
  }

  for (it =  AddressesNeedingSourceLine.begin() ;
       it != AddressesNeedingSourceLine.end() ;
       it++ ) {
    fprintf(tmpfile, "0x%08x\n", *it);
  }

  fclose( tmpfile );

  /*
   *  Generate a file with the addr2line mapping
   */
  if ( verbose )
    fprintf( stderr, "Running addr2line\n" );

  {
    char command[512];
    sprintf(
      command,
      "%s -e %s <%s | dos2unix | sed "
          "-e 's/^.*cpukit\\/.*\\/src\\///' -e 's/^.*include\\/.*\\/.*\\///'"
      ">%s",
      Tools->getAddr2line(),
      executable,
      "ranges.tmp",
      "ranges01.tmp"
    );
    if ( system( command ) ) {
      fprintf( stderr, "addr2line command (%s) failed\n", command );
      exit( -1 );
    }
  }

  // system( "rm -f ranges.tmp" );

  /*
   *  Go back over the ranges, read the addr2line output, and correlate it.
   */

  if ( verbose )
    fprintf( stderr, "Merging addr2line output into range\n" );

  tmpfile = fopen( "ranges01.tmp", "r" );
  if ( !tmpfile ) {
    fprintf( stderr, "Unable to open %s\n\n", "ranges01.tmp" );
    exit(-1);
  }

  for (it =  AddressesNeedingSourceLine.begin() ;
       it != AddressesNeedingSourceLine.end() ;
       it++ ) {
    char buffer[512];
    char *cStatus;

    cStatus = fgets( buffer, 512, tmpfile );
    if ( cStatus == NULL ) {
      fprintf( stderr, "Out of sync in addr2line output\n" );
      exit( -1 );
    }
    buffer[ strlen(buffer) - 1] = '\0';

    CoverageMap->setSourceLine( *it, std::string( buffer ) );
  }
  fclose( tmpfile );

  // system( "rm -f ranges01.tmp" );
  // system( "rm -f ranges.tmp" );
}

/*
 *  Write coverage report
 */
void WriteCoverageReport()
{
  FILE *report;
  std::list<Coverage::CoverageRange>::iterator it;

  /*
   *  Now begin to write the real report
   */
  report = fopen( coverageReportFile, "w" );

  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", coverageReportFile );
    usage();
    exit(-1);
  }

  if ( verbose )
    fprintf( stderr, "Writing Report\n" );

  for (it =  Ranges->Set.begin() ;
       it != Ranges->Set.end() ;
       it++ ) {
    const Coverage::Explanation *explanation;
    std::string lowLine;
    std::string highLine;

    lowLine  = CoverageMap->sourceLine( it->lowAddress );
    highLine = CoverageMap->sourceLine( it->highAddress );

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

  /*
   *  Let the user know how many cases there were
   */
  printf( "%d uncovered ranges found\n", UncoveredCases );
}

/*
 *  Write size report
 */
void WriteSizeReport(void)
{
  FILE *report;
  std::list<Coverage::CoverageRange>::iterator it;

  /*
   *  Now begin to write the real report
   */
  report = fopen( sizeReportFile, "w" );

  if ( !report ) {
    fprintf( stderr, "Unable to open %s\n\n", sizeReportFile );
    usage();
    exit(-1);
  }

  if ( verbose )
    fprintf( stderr, "Writing Report\n" );

  for (it =  Ranges->Set.begin() ;
       it != Ranges->Set.end() ;
       it++ ) {
    fprintf(
      report,
      "%s\t%d\n",
      CoverageMap->sourceLine( it->lowAddress ).c_str(),
      it->highAddress - it->lowAddress + 1
    );
  }

  fclose( report );

}

#define PrintableString(_s) \
       ((!(_s)) ? "NOT SET" : (_s))

int main(
  int argc,
  char **argv
)
{
  int opt;
  int i;
  char *format = NULL;

  progname = argv[0];

  while ((opt = getopt(argc, argv, "a:e:E:f:h:l:m:r:s:T:v")) != -1) {
    switch (opt) {
      case 'T': target             = optarg;  break;
      case 'e': executable         = optarg;  break;
      case 'E': explanations       = optarg;  break;
      case 'm': mergedCoverageFile = optarg;  break;
      case 'r': coverageReportFile = optarg;  break;
      case 's': sizeReportFile     = optarg;  break;
      case 'v': verbose            = 1;       break;
      case 'f':
        coverageFormat = Coverage::CoverageFormatToEnum(optarg);
        format = optarg;
        break;
      case 'l':
        if ( ! stringToUint32( optarg, 16, &lowAddress ) ) {
          fprintf( stderr, "Low address is not a hexadecimal number\n" );
          usage();
          exit(-1);
        }
        break;
      case 'h': 
        if ( ! stringToUint32( optarg, 16, &highAddress ) ) {
          fprintf( stderr, "High address is not a hexadecimal number\n" );
          usage();
          exit(-1);
        }
        break;
      default: /* '?' */
        usage();
        exit( -1 );
    }
  }
  if ( verbose ) {
    fprintf( stderr, "verbose         : %d\n", verbose );
    fprintf( stderr, "Coverage Format : %s\n", format );
    fprintf( stderr, "low address     : 0x%08x\n", lowAddress );
    fprintf( stderr, "high address    : 0x%08x\n", highAddress );
    fprintf( stderr, "Target          : %s\n", PrintableString(target) );
    fprintf( stderr, "executable      : %s\n", PrintableString(executable) );
    fprintf( stderr, "merged coverage : %s\n",
             PrintableString(mergedCoverageFile) );
    fprintf( stderr, "\n" );
  }

  /*
   * Target name must be set
   */
  if ( !target ) {
    fprintf( stderr, "target must be given.\n\n" );
    usage();
    exit(-1);
  }

  Tools = new Coverage::Toolnames( target );

  /*
   *  Validate format
   */
  if ( !format ) {
    fprintf( stderr, "coverage format report must be given.\n\n" );
    usage();
    exit(-1);
  }

  /*
   *  Create a ranges set
   */

  Ranges = new Coverage::CoverageRanges();
  Explanations = new Coverage::Explanations();

  Explanations->load( explanations );

  /*
   * Validate address range
   */
  if ( lowAddress == 0xffffffff ) {
    fprintf( stderr, "Low address not specified.\n\n" );
    usage();
    exit(-1);
  }

  if ( highAddress == 0xffffffff ) {
    fprintf( stderr, "High address not specified.\n\n" );
    usage();
    exit(-1);
  }

  if ( lowAddress >= highAddress ) {
    fprintf( stderr, "Low address >= high address.\n\n" );
    usage();
    exit(-1);
  }

  /*
   * Create coverage map
   */
  CoverageMap = new Coverage::CoverageMap( lowAddress, highAddress );
  if ( !CoverageMap ) {
    fprintf( stderr, "Unable to create coverage map.\n\n" );
    exit(-1);
  }

  /*
   * Create input
   */
  CoverageReader = Coverage::CreateCoverageReader(coverageFormat);
  if ( !CoverageReader ) {
    fprintf( stderr, "Unable to create coverage file reader.\n\n" );
    exit(-1);
  }

  /*
   * Create the objdump processor
   */
  ObjdumpProcessor = new Coverage::ObjdumpProcessor();

  /*
   * Create writer
   *
   * NOTE: We ALWAYS write the merged coverage in RTEMS format.
   */
  CoverageWriter = Coverage::CreateCoverageWriter(
    Coverage::COVERAGE_FORMAT_RTEMS
  );
  if ( !CoverageWriter ) {
    fprintf( stderr, "Unable to create coverage file writer.\n\n" );
    exit(-1);
  }

  /*
   * Add in the objdump before reading the coverage information.  We may
   * want to take advantage of the information line where instructions
   * begin.
   */
  if ( executable ) {
    if ( verbose )
      fprintf( stderr, "Reading objdump of %s\n", executable );
    ObjdumpProcessor->initialize( executable, CoverageMap );
  }

  /*
   * Now get to some real work
   */
  if ( verbose )
    fprintf( stderr, "Processing coverage files\n" );
  for ( i=optind ; i < argc ; i++ ) {
    //fprintf( stderr, "Processing %s\n", argv[i] );
    CoverageReader->ProcessFile( argv[i], CoverageMap );
  } 

  /*
   * Now to write some output
   */
  if ( mergedCoverageFile ) {
    if ( verbose )
      fprintf(
        stderr,
        "Writing merged coverage file (%s)\n",
        mergedCoverageFile
      );
    CoverageWriter->writeFile(
      mergedCoverageFile,
      CoverageMap,
      lowAddress,
      highAddress
    );
  }

  /*
   * Iterate over the coverage map and determine the uncovered ranges.
   */
  ComputeUncoveredRanges();

  /*
   *  Look up the source file and line number for the addresses
   *  of interest.
   */ 
  FindSourceForAddresses();

  /*
   *  Report of ranges not executed
   */ 
  if ( coverageReportFile ) {
    if ( verbose )
      fprintf( stderr, "Writing coverage report (%s)\n", coverageReportFile );
    WriteCoverageReport();
  }

  /*
   *  Simple formatted report of size of ranges 
   */ 
  if ( sizeReportFile ) {
    if ( verbose )
      fprintf( stderr, "Writing size report (%s)\n", sizeReportFile );
    WriteSizeReport();
  }

  /*
   *  Generate annotated assembly file
   */
  ObjdumpProcessor->writeAnnotated(
    CoverageMap,
    lowAddress,
    highAddress,
    "annotated.txt",
    "annotated.html"
  );

  /*
   * write explanations that were not found
   */
  std::string str = explanations;
  str = str + ".NotFound";
  if ( verbose )
    fprintf( stderr, "Writing Not Found Report (%s)\n", str.c_str() );
  Explanations->writeNotFound(str.c_str());

  /*
   * Calculate coverage percentage
   */
  {
    uint32_t a;
    uint32_t notExecuted = 0;
    double   percentage;

    for ( a=lowAddress ; a < highAddress ; a++ ) {
      if ( !CoverageMap->wasExecuted( a ) )
        notExecuted++;
    }

    percentage = (double) notExecuted;
    percentage /= (double) (highAddress - lowAddress);
    percentage *= 100.0;
    printf( "Bytes Analyzed          : %d\n", highAddress - lowAddress );
    printf( "Bytes Not Executed      : %d\n", notExecuted );
    printf( "Percentage Executed     : %5.4g\n", 100.0 - percentage  );
    printf( "Percentage Not Executed : %5.4g\n", percentage  );
  }

  return 0;
}
