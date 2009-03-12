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

namespace Coverage {

  Toolnames::Toolnames(
    const char *target_arg
  )
  {
    target    = target_arg; 
    addr2line = target + "-addr2line";
    objdump   = target + "-objdump";
  }

  Toolnames::~Toolnames()
  {
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
