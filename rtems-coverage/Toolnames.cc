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
    int i;

    target    = target_arg; 
    addr2line = target + "-addr2line";
    objdump   = target + "-objdump";
    for (i=0 ; target[i] && target[i] != '-' ; ) {
      cpu[i]   = target[i];
      cpu[++i] = '\0';
    }
  }

  Toolnames::~Toolnames()
  {
  }

  const char *Toolnames::getCPU( void )
  {
    return cpu.c_str();
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

}
