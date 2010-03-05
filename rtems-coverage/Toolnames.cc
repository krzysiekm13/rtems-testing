/*
 *  $Id$
 */

/*! @file Toolnames.cc
 *  @brief Toolnames Implementation
 *
 *  This file contains the implementation of the functions 
 *  which provide a base level of functionality of a CoverageMap.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Toolnames.h"

namespace Coverage {

  Toolnames::Toolnames(
    const char* const target_arg
  )
  {
    int i;

    target    = target_arg; 
    addr2line = target + "-addr2line";
    nm        = target + "-nm";
    objdump   = target + "-objdump";
    for (i=0 ; target[i] && target[i] != '-' ; ) {
      cpu[i]   = target[i];
      cpu[++i] = '\0';
    }
  }

  Toolnames::~Toolnames()
  {
  }

  const char* Toolnames::getAddr2line() const
  {
    return addr2line.c_str();
  }

  const char* Toolnames::getCPU( void ) const
  {
    return cpu.c_str();
  }

  const char* Toolnames::getNm() const
  {
    return nm.c_str();
  }

  const char* Toolnames::getObjdump() const
  {
    return objdump.c_str();
  }

  const char* Toolnames::getTarget( void ) const
  {
    return target.c_str();
  }

}
