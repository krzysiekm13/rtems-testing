/*
 *   $Id$
 */

#ifndef __APP_COMMON_h
#define __APP_COMMON_h

#include <list>

#include "DesiredSymbols.h"
#include "Explanations.h"
#include "Toolnames.h"

extern Coverage::Explanations*   AllExplanations;
extern Coverage::DesiredSymbols* SymbolsToAnalyze;
extern Coverage::Toolnames*      Tools;
extern bool                      Verbose;

#endif
