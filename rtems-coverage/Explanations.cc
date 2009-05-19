/*
 *  $Id$
 */

/*! @file Explanations.cc
 *  @brief Explanations Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a Explanations.
 */

#include "Explanations.h"
#include <stdlib.h>
#include <string.h>

namespace Coverage {

  Explanations::Explanations()
  {
    NotFoundOccurred = false;
  }

  Explanations::~Explanations()
  {
  }

  void Explanations::load(
    const char *explanations
  )
  {
    FILE       *explain;
    char        buffer[512];
    char       *cStatus;
    Explanation  e;
    int          line = 1;

    if ( !explanations )
      return;

    NotFoundOccurred = false;
    explain = fopen( explanations, "r" );
    if ( !explain ) {
      fprintf(
        stderr,
        "Unable to open explanations file %s\n\n",
        explanations
      );
      exit(-1);
    }

    while ( 1 ) {
      // Read the starting line
      cStatus = fgets( buffer, 512, explain );
      if ( cStatus == NULL ) {
        break;
      }
      buffer[ strlen(buffer) - 1] = '\0';

      // Have we already seen this one?
      if ( Set.find( buffer ) != Set.end() ) {
        fprintf(
          stderr,
          "File: %s, Line %d: Duplicate explanation: %s\n",
          explanations,
          line,
          buffer
        );
        exit( -1 );
      }

      // Add the starting line and file
      e.startingPoint = std::string(buffer);
      e.found = false;
      line++;

      // Get the classification 
      cStatus = fgets( buffer, 512, explain );
      if ( cStatus == NULL ) {
        fprintf(
          stderr,
          "File %s: Line %d: Out of sync at the classification\n",
          explanations,
          line
        );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';
      e.classification = buffer;
      line++;

      // Get the explanation 
      cStatus = fgets( buffer, 512, explain );
      if ( cStatus == NULL ) {
        fprintf(
          stderr,
          "File %s: Line %d: Out of sync at the explanation\n",
          explanations,
          line
        );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';
      e.explanation = buffer;
      line++;

      // Add this to the Set of Explanations
      Set[ e.startingPoint ] = e;
    }
  }

  void Explanations::writeNotFound(
    const char *fileName
  )
  {
    FILE *notFoundFile;

    if ( !fileName )
      return;
   
    notFoundFile = fopen( fileName, "w" );
    if ( !fileName ) {
      fprintf( stderr, "Unable to open File for unfound explanations %s\n\n", fileName );
      exit( -1 );
    }
    
    for( std::map<std::string, Explanation>::iterator itr = Set.begin();
         itr != Set.end();
         itr++ ) {
      Explanation e = (*itr).second;
      std::string key = (*itr).first;
 
      if ( !e.found ) {
        NotFoundOccurred = true;
        fprintf( notFoundFile,"%s\n%s\n", e.startingPoint.c_str(), e.explanation.c_str() );
      } 
    }
    fclose( notFoundFile );

    if ( !NotFoundOccurred ){
      if ( !unlink( fileName )) {
        fprintf( stderr, 
          "Warning: Unable to unlink %s\n\n", 
          fileName 
        );
      }
    } 
  }

  std::string Explanations::lookupClassification(
    std::string start
  )
  {
    if ( Set.find( start ) == Set.end() )
      return "no classification";
    return Set[ start ].classification;
  }

  std::string Explanations::lookupExplanation(
    std::string start
  )
  {
    if ( Set.find( start ) == Set.end() )
      return "no explanation";
    Set[ start ].found = true;
    return Set[ start ].explanation;
  }

}
