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
    NotFoundOccured = false;
  }

  Explanations::~Explanations()
  {
  }

  void Explanations::load(
    const char *explanations
  )
  {
    FILE *explain;
    char buffer[512];
    char *cStatus;
    Explanation e;

    if ( !explanations )
      return;

    NotFoundOccured = false;
    explain = fopen( explanations, "r" );
    if ( !explain ) {
      fprintf( stderr, "Unable to open explanations file %s\n\n", explanations );
      exit(-1);
    }

    while ( 1 ) {
      cStatus = fgets( buffer, 512, explain );
      if ( cStatus == NULL ) {
        break;
      }
      buffer[ strlen(buffer) - 1] = '\0';

      if ( Set.find( buffer ) != Set.end() ) {
        fprintf( stderr, "Duplicate explanation: %s\n", buffer );
        exit( -1 );
      }
      e.startingPoint = std::string(buffer);
      e.found = false;

      cStatus = fgets( buffer, 512, explain );
      if ( cStatus == NULL ) {
        fprintf( stderr, "Out of sync in explanations file %s\n", explanations );
        exit( -1 );
      }
      buffer[ strlen(buffer) - 1] = '\0';

      e.explanation = buffer;

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
        NotFoundOccured = true;
        fprintf( notFoundFile,"%s\n%s\n", e.startingPoint.c_str(), e.explanation.c_str() );
      } 
    }
    fclose( notFoundFile );

    if ( !NotFoundOccured ){
      if ( !unlink( fileName )) {
        fprintf( stderr, 
          "Warning: Unable to unlink %s\n\n", 
          fileName 
        );
      }
    } 
  }

  std::string Explanations::lookup(
    std::string start
  )
  {
    if ( Set.find( start ) == Set.end() )
      return "no explanation";
    Set[ start ].found = true;
    return Set[ start ].explanation;
  }

}
