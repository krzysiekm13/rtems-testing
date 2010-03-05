/*
 *  $Id$
 */

/*! @file Explanations.cc
 *  @brief Explanations Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a Explanations.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Explanations.h"

namespace Coverage {

  Explanations::Explanations()
  {
  }

  Explanations::~Explanations()
  {
  }

  void Explanations::load(
    const char* const explanations
  )
  {
    #define MAX_LINE_LENGTH 512
    FILE       *explain;
    char        buffer[MAX_LINE_LENGTH];
    char        *cStatus;
    Explanation *e;
    int          line = 1;

    if (!explanations)
      return;

    explain = fopen( explanations, "r" );
    if (!explain) {
      fprintf(
        stderr,
        "ERROR: Explanations::load - unable to open explanations file %s\n",
        explanations
      );
      exit(-1);
    }

    while ( 1 ) {
      e = new Explanation;
      // Read the starting line of this explanation and
      // skip blank lines between
      do {
        buffer[0] = '\0';
        cStatus = fgets( buffer, MAX_LINE_LENGTH, explain );
        if (cStatus == NULL) {
          goto done;
        }
        buffer[ strlen(buffer) - 1] = '\0';
        line++;
      } while ( buffer[0] == '\0' );

      // Have we already seen this one?
      if (set.find( buffer ) != set.end()) {
        fprintf(
          stderr,
          "ERROR: Explanations::load - line %d "
          "contains a duplicate explanation (%s)\n",
          line,
          buffer
        );
        exit( -1 );
      }

      // Add the starting line and file
      e->startingPoint = std::string(buffer);
      e->found = false;

      // Get the classification 
      cStatus = fgets( buffer, MAX_LINE_LENGTH, explain );
      if (cStatus == NULL) {
        fprintf(
          stderr,
          "ERROR: Explanations::load - line %d "
          "out of sync at the classification\n",
          line
        );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';
      e->classification = buffer;
      line++;

      // Get the explanation 
      while (1) {
        cStatus = fgets( buffer, MAX_LINE_LENGTH, explain );
        // fprintf( stderr, "%d - %s\n", line, buffer );
        if (cStatus == NULL) {
          fprintf(
            stderr,
            "ERROR: Explanations::load - line %d "
            "out of sync at the explanation\n",
            line
          );
          exit( -1 );
        }
        buffer[ strlen(buffer) - 1] = '\0';
        line++;

        const char delimiter[4] = "+++";
        if (!strncmp( buffer, delimiter, 3 )) {
          break;
        }
        // XXX only taking last line.  Needs to be a vector
        e->explanation.push_back( buffer );
      }

      // Add this to the set of Explanations
      set[ e->startingPoint ] = *e;
    }
done:
    ;

    #undef MAX_LINE_LENGTH
  }

  const Explanation *Explanations::lookupExplanation(
    std::string& start
  )
  {
    if (set.find( start ) == set.end()) {
      #if 0
        fprintf( stderr, 
          "Warning: Unable to find explanation for %s\n", 
          start.c_str() 
        );
      #endif
      return NULL;
    }
    set[ start ].found = true;
    return &set[ start ];
  }

  void Explanations::writeNotFound(
    const char* const fileName
  )
  {
    FILE* notFoundFile;
    bool  notFoundOccurred = false;

    if (!fileName)
      return;
   
    notFoundFile = fopen( fileName, "w" );
    if (!fileName) {
      fprintf(
        stderr,
        "ERROR: Explanations::writeNotFound - unable to open file %s\n",
        fileName
      );
      exit( -1 );
    }
    
    for (std::map<std::string, Explanation>::iterator itr = set.begin();
         itr != set.end();
         itr++) {
      Explanation e = (*itr).second;
      std::string key = (*itr).first;
 
      if (!e.found) {
        notFoundOccurred = true;
        fprintf(
          notFoundFile,
          "%s\n",
          e.startingPoint.c_str()
        );
      } 
    }
    fclose( notFoundFile );

    if (!notFoundOccurred) {
      if (!unlink( fileName )) {
        fprintf( stderr, 
          "Warning: Unable to unlink %s\n\n", 
          fileName 
        );
      }
    } 
  }

}
