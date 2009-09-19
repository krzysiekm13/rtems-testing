/*
 *  $Id$
 */

/*! @file Toolnames.cc
 *  @brief Toolnames Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a CoverageMap.
 */

#include "Toolnames.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

namespace Coverage {

  Toolnames::Toolnames(
    const char *target_arg
  )
  {
    target    = target_arg; 
    addr2line = target + "-addr2line";
    objdump   = target + "-objdump";
    if ( !strncmp( target_arg, "sparc", 5 ) ) {
      nopSize = 4;
    } else if ( !strncmp( target_arg, "i386", 4 ) ) {
      nopSize = 1;
    } else if ( !strncmp( target_arg, "arm", 3 ) ) {
      // This is right for ARM mode, not Thumb mode.
      nopSize = 4;
    } else {
      fprintf( stderr, "HOW LARGE IS NOP ON THIS ARCHITECTURE? -- fix me\n" );
      exit(1);
    }
  }

  Toolnames::~Toolnames()
  {
  }

  const char *Toolnames::getTarget( void )
  {
    return target.c_str();
  }

  const char *Toolnames::getAddr2line()
  {
    return addr2line.c_str();
  }

  const char *Toolnames::getObjdump()
  {
    return objdump.c_str();
  }

  const int Toolnames::getNopSize( void )
  {
     return nopSize;
  }
  
}
