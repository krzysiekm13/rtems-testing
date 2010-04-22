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

namespace Target {

  TargetBase::TargetBase( std::string targetName ):
    targetName_m( targetName )
  {
  }

  TargetBase::~TargetBase()
  {
  }

}
