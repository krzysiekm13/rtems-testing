/*
 *  $Id$
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "app_common.h"

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


