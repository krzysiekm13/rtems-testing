/*
 *  $Id$
 */

/*! @file TraceReaderLogQEMU.cc
 *  @brief TraceReaderLogQEMU Implementation
 *
 *  This file contains the implementation of the functions supporting
 *  reading the QEMU coverage data files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <string.h>

#include "qemu-log.h"

#include "TraceReaderLogQEMU.h"
#include "TraceList.h"
#include "ObjdumpProcessor.h"
#include "Toolnames.h"
#include "app_common.h"

int main(
  int    argc,
  char** argv
)
{
  Trace::TraceReaderLogQEMU    log;
   
  // Create toolnames.
  Tools = new Coverage::Toolnames( "i386" );

  objdumpProcessor = new Coverage::ObjdumpProcessor();
 
  objdumpProcessor->loadAddressTable( "static" );

  log.processFile( "/tmp/qemu.log" );

  log.Trace.ShowList();
}
