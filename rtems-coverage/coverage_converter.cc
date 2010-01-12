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
#include "app_common.h"

/*
 *  Variables to control global behavior
 */
int verbose = 0;
Coverage::CoverageFormats_t inputFormat;
uint32_t lowAddress  = 0xffffffff;
uint32_t highAddress = 0xffffffff;

char *progname;

/*
 *  Global variables for the program
 */
Coverage::CoverageMapBase    *CoverageMap    = NULL;
Coverage::CoverageReaderBase *CoverageReader = NULL;
Coverage::CoverageWriterBase *CoverageWriter = NULL;

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
    "Usage: %s [-v] -l ADDRESS -h ADDRESS coverage_in coverage_out\n"
    "\n"
    "  -l low address   - low address of range to merge\n"
    "  -l high address  - high address of range to merge\n"
    "  -f format        - coverage files are in <format> (Qemu)\n"
    "  -v               - verbose at initialization\n"
    "\n",
    progname
  );
}

#define PrintableString(_s) \
       ((!(_s)) ? "NOT SET" : (_s))

int main(
  int argc,
  char **argv
)
{
  int         opt;
  char       *format = NULL;
  const char *coverageIn;
  const char *coverageFile;

  progname = argv[0];

  while ((opt = getopt(argc, argv, "f:h:l:v")) != -1) {
    switch (opt) {
      case 'v': verbose = 1;       break;
      case 'f':
        inputFormat = Coverage::CoverageFormatToEnum(optarg);
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

  if ( (argc - optind) != 2 ) {
    fprintf( stderr, "ERROR -- Must provide input and output files\n" );
    exit(1);
  }

  coverageIn   = argv[optind];
  coverageFile = argv[optind + 1];

  if ( verbose ) {
    fprintf( stderr, "verbose       : %d\n", verbose );
    fprintf( stderr, "Input Format  : %s\n", format );
    fprintf( stderr, "Input File    : %s\n", coverageIn );
    fprintf( stderr, "Output Format : %s\n", "RTEMS" );
    fprintf( stderr, "Output File   : %s\n", coverageFile );
    fprintf( stderr, "low address   : 0x%08x\n", lowAddress );
    fprintf( stderr, "high address  : 0x%08x\n", highAddress );
    fprintf( stderr, "\n" );
  }

  /*
   *  Validate format
   */
  if ( !format ) {
    fprintf( stderr, "input format must be given.\n\n" );
    usage();
    exit(-1);
  }

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
   * Create writer
   */
  CoverageWriter =
    Coverage::CreateCoverageWriter(Coverage::COVERAGE_FORMAT_RTEMS);
  if ( !CoverageWriter ) {
    fprintf( stderr, "Unable to create coverage file writer.\n\n" );
    exit(-1);
  }

  CoverageReader = CreateCoverageReader( inputFormat );
  if ( !CoverageReader ) {
    fprintf( stderr, "Unable to create input file reader.\n\n" );
    exit(-1);
  }

  /*
   * Now get to some real work
   */
  if ( verbose )
    fprintf( stderr, "Processing %s\n", coverageIn );
  CoverageReader->ProcessFile( coverageIn, CoverageMap );

  if ( verbose )
    fprintf(
      stderr, "Writing coverage file (%s)\n", coverageFile );
  CoverageWriter->writeFile(
    coverageFile,
    CoverageMap,
    lowAddress,
    highAddress
  );

  return 0;
}
