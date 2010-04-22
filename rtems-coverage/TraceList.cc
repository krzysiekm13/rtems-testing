/*
 *  $Id$
 */

#include "TraceList.h"
#include <stdio.h>

namespace Trace {

    TraceList::TraceList()
    {
    }

    TraceList::~TraceList() 
    { 
    }

    void TraceList::add(
      uint32_t         lowAddressArg,
      uint32_t         highAddressArg,
      exitReason_t     why
    )
    {
      traceRange_t t;

      t.lowAddress = lowAddressArg;
      t.length = highAddressArg - lowAddressArg;
      t.exitReason = why;

      set.push_back( t );     
    }

    void  TraceList::ShowList()
    {
      ranges_t::iterator   ritr;

      for ( ritr=set.begin(); ritr != set.end(); ritr++ ) {
        printf(
          "Start 0x%x, length 0x%03x Reason %d\n", 
          ritr->lowAddress, 
          ritr->length, 
          ritr->exitReason 
        );
      }
    }

}
