/*
 *  $Id$
 */

/*! @file Target_lm32.cc
 *  @brief Target_lm32 Implementation
 *
 *  This file contains the implementation of the base class for 
 *  functions supporting target unique functionallity.
 */
#include "Target_lm32.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace Target {

  Target_lm32::Target_lm32( std::string targetName ):
    TargetBase( targetName )
  {
  }

  Target_lm32::~Target_lm32()
  {
  }

  bool Target_lm32::isNopLine(
    const char* const line,
    int&              size
  )
  {
    if (!strcmp( &line[strlen(line)-3], "nop")) {
      size = 4; 
      return true;
    }

    return false;
  }

  bool Target_lm32::isBranch(
      const char* const instruction
  )
  {
    fprintf( stderr, "DETERMINE BRANCH INSTRUCTIONS FOR THIS ARCHITECTURE! -- fix me\n" );
    exit( -1 );    
  }

  TargetBase *Target_lm32_Constructor(
    std::string          targetName
  )
  {
    return new Target_lm32( targetName );
  }

}
