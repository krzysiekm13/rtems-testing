/*
 *   $Id$
 */


/*! @file ConfigFile.cc
 *  @brief ConfigFile Implementation
 *
 *  This file contains the implementation of the FileReader class.
 */

#include "ConfigFile.h"
#include <string.h>
#include <stdio.h>

namespace Configuration {

  FileReader::FileReader(
    Options_t *options
  )
  {
    options_m = options;
  }

  FileReader::~FileReader()
  {
  }

  bool FileReader::processFile(
    const char* const     file
  )
  {
    #define METHOD "FileReader::processFile - "
    FILE *in;
    char  line[256];
    char  option[256];
    char  value[256];
    int   line_no;

    if ( file == NULL ) {
      fprintf( stderr, METHOD "NULL filename\n" );
      return false;
    }

    in = fopen( file, "r" );
    if ( !in ) {
      fprintf( stderr, METHOD "unable to open %s\n", file );
      return false;
    }

    line_no = 0;
    while (fgets(line, sizeof(line), in) != NULL) {

      line_no++;

      /* Ignore empty lines and comments */
      if (line[0] == '#' || line[0] == '\n')
        continue;

      if (sscanf(line, "%s = %[^ \r\n#]", option, value) != 2) {
        fprintf(
          stderr,
          "%s: line %d is invalid: %s",
          file,
          line_no,
          line
        );
        continue;
      }

      if ( !setOption(option, value) ) {
        fprintf(
          stderr,
          "%s: line %d: option %s is unknown\n",
          file,
          line_no,
          option
        );
        continue;
      }

    }

    return false;
  }

  bool FileReader::setOption(
    const char* const option,
    const char* const value
  )
  {
    Options_t *o;

    for ( o=options_m ; o->option ; o++ ) {
      if ( !strcmp( o->option, option ) ) {
        o->value = strdup( value );
        return true;
      }
    }
    return false;
  }

  const char *FileReader::getOption(
    const char* const option
  )
  {
    Options_t *o;

    for ( o=options_m ; o->option ; o++ ) {
      if ( !strcmp( o->option, option ) ) {
        return o->value;
      }
    }
    return NULL;
  }

  void FileReader::printOptions(void)
  {
    Options_t *o;

    for ( o=options_m ; o->option ; o++ ) {
      fprintf( stderr, "(%s)=(%s)\n", o->option, o->value );
    }
  }
}
