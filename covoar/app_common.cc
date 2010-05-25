/*
 *  $Id$
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "app_common.h"
#include "DesiredSymbols.h"
#include "Explanations.h"

/* hack so this can compile on the RH7 RTEMS 4.5 host */
#if (__GNUC__ <= 2)
#define STAT stat
#define OPEN fopen
#else
#define STAT stat64
#define OPEN fopen64
#endif

/*
 *  Global variables for the program
 */
Coverage::Explanations*     AllExplanations     = NULL;
Coverage::ObjdumpProcessor* objdumpProcessor    = NULL;
Coverage::DesiredSymbols*   SymbolsToAnalyze    = NULL;
bool                        Verbose             = false;
const char*                 outputDirectory     = ".";
bool                        BranchInfoAvailable = false;
Target::TargetBase*         TargetInfo          = NULL;
const char*                 dynamicLibrary      = NULL;
const char*                 projectName         = NULL;
char                        inputBuffer[MAX_LINE_LENGTH];


bool FileIsNewer(
  const char *f1,
  const char *f2
)
{
  struct STAT buf1, buf2;

   if (STAT( f2, &buf2 ) == -1)
    return true;
  
  if (STAT( f1, &buf1 ) == -1)
    exit (1);

  if (buf1.st_mtime > buf2.st_mtime)
    return true;

  return false;
}

bool FileIsReadable( const char *f1 )
{
  struct STAT buf1;

  if (STAT( f1, &buf1 ) == -1)
    return false;

  if (buf1.st_size == 0)
    return false;

  // XXX check permission ??
  return true;
}

bool ReadUntilFound( FILE *file, const char *line )
{
  char discardBuff[100];
  size_t  len = strlen( line );

  do { 
    if ( !fgets( discardBuff, 99, file ) )
      return false;

    if ( strncmp( discardBuff, line, len ) == 0 ) 
      return true; 
  } while (1);
}

