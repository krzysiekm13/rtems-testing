/*
 *  $Id$
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "app_common.h"
#include "DesiredSymbols.h"
#include "Explanations.h"

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

bool FileIsNewer( const char *f1, const char *f2 ) {
  struct stat buf1, buf2;

   if (stat( f2, &buf2 ) == -1)
    return true;
  
  if (stat (f1, &buf1 ) == -1)
    exit (1);

  if (buf1.st_mtime > buf2.st_mtime)
    return true;

  return false;
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

