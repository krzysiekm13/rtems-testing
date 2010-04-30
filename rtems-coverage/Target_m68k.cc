/*
 *  $Id$
 */

/*! @file Target_m68k.cc
 *  @brief Target_m68k Implementation
 *
 *  This file contains the implementation of the base class for 
 *  functions supporting target unique functionallity.
 */
#include "Target_m68k.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

namespace Target {

  Target_m68k::Target_m68k( std::string targetName ):
    TargetBase( targetName )
  {
    branchInstructions.push_back("bcc");
    branchInstructions.push_back("bcs");
    branchInstructions.push_back("beq");
    branchInstructions.push_back("bge");
    branchInstructions.push_back("bgt");
    branchInstructions.push_back("bhi");
    branchInstructions.push_back("bhs");
    branchInstructions.push_back("ble");
    branchInstructions.push_back("blo");
    branchInstructions.push_back("bls");
    branchInstructions.push_back("blt");
    branchInstructions.push_back("bmi");
    branchInstructions.push_back("bne");
    branchInstructions.push_back("bpl");
    branchInstructions.push_back("bvc");
    branchInstructions.push_back("bvs");
  
    branchInstructions.sort();

  }

  Target_m68k::~Target_m68k()
  {
  }

  bool Target_m68k::isNopLine(
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

  bool Target_m68k::isBranch(
      const char* const instruction
  )
  {
    fprintf( stderr, "DETERMINE BRANCH INSTRUCTIONS FOR THIS ARCHITECTURE! -- fix me\n" );
    exit( -1 );    
  }

  TargetBase *Target_m68k_Constructor(
    std::string          targetName
  )
  {
    return new Target_m68k( targetName );
  }

}
