/*
 *  $Id$
 */

/*! @file TargetBase.cc
 *  @brief TargetBase Implementation
 *
 *  This file contains the implementation of the base class for 
 *  functions supporting target unique functionallity.
 */
#include "TargetBase.h"
#include <algorithm>
#include <stdio.h>

namespace Target {

  TargetBase::TargetBase( 
    std::string targetName
  ):
    targetName_m( targetName )
  {
  }

  TargetBase::~TargetBase()
  {
  }


  bool TargetBase::isBranch(
      const char* const instruction
  )
  {
    if (branchInstructions.empty()) {
      fprintf( 
        stderr,
        "DETERMINE BRANCH INSTRUCTIONS FOR THIS ARCHITECTURE! -- fix me\n" 
       );
       exit( -1 );    
    }
    
    if ( find(
           branchInstructions.begin(), 
           branchInstructions.end(), 
           instruction 
        ) == branchInstructions.end()
    )
      return false;
    return true;
  }

  bool TargetBase::isBranchLine(
    const char* const line
  )
  {
    const char *ch;
    char instruction[120];
    int  result;

    
    ch = &(line[0]);

    // Increment to the first tab in the line
    while ((*ch != '\t') && (*ch != '\0')) { 
      ch++;
    }
    if (*ch != '\t') {
      fprintf( 
        stderr,
        "ERROR: TargetBase::isBranchLine - Unable to find instruction\n"
      );
      exit( -1 );    
    }
    ch++;

    // Increment to the second tab in the line
    while ((*ch != '\t') && (*ch != '\0')) 
      ch++;
    if (*ch != '\t') {
      fprintf( 
        stderr,
        "ERROR: TargetBase::isBranchLine - Unable to find instruction\n"
      );
      exit( -1 );    
    }
    ch++;

    // Grab the instruction which is the next word in the buffer after the second tab.
    result = sscanf( ch, "%s", instruction );
    if (result != 1) {
        fprintf(
          stderr,
          "ERROR: TargetBase::isBranchLine - Unable to find instruction\n"
        );
        exit( -1 );
    }

    return isBranch( instruction );
  }
}
