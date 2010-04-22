/*
 *  $Id$
 */

/*! @file Target_sparc.cc
 *  @brief Target_sparc Implementation
 *
 *  This file contains the implementation of the base class for 
 *  functions supporting target unique functionallity.
 */
#include "Target_sparc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace Target {

  Target_sparc::Target_sparc( std::string targetName ):
    TargetBase( targetName )
  {
  }

  Target_sparc::~Target_sparc()
  {
  }

  bool Target_sparc::isNopLine(
    const char* const line,
    int&              size
  )
  {
    if (!strcmp( &line[strlen(line)-3], "nop")) {
      size = 2;
      return true;
    }

    #define GNU_LD_FILLS_ALIGNMENT_WITH_RTS
    #if defined(GNU_LD_FILLS_ALIGNMENT_WITH_RTS)
      // Until binutils 2.20, binutils would fill with rts not nop
      if (!strcmp( &line[strlen(line)-3], "rts")) {
        size = 4; 
        return true;
      } 
    #endif

    return false;
  }

  bool Target_sparc::isBranch(
      const char* const instruction
  )
  {
    fprintf( stderr, "DETERMINE BRANCH INSTRUCTIONS FOR THIS ARCHITECTURE! -- fix me\n" );
    exit( -1 );    
  }

  TargetBase *Target_sparc_Constructor(
    std::string          targetName
  )
  {
    return new Target_sparc( targetName );
  }
}
