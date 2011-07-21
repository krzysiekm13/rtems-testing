/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <timesys.h>
#include <rtems/timerdrv.h>
#include "test_support.h"

void benchmark_NAME_OF_OPERATION(void)
{
  long end_time;
  int  status;

  benchmark_timer_initialize();
    /* XXX single shot operation goes here -- test expected status outside */
    /* XXX of the benchmark'ed region */
  end_time = benchmark_timer_read();
  rtems_test_assert( status == 0 );

  put_time(
    "@DESC@",
    end_time,
    1,        /* Only executed once */
    0,
    0
  );

}

void *POSIX_Init(
  void *argument
)
{

  puts( "\n\n*** POSIX TIME TEST @UPPER@ ***" );

  /* XXX any required initialization goes here */

  benchmark_NAME_OF_OPERATION();
  
  puts( "*** END OF POSIX TIME TEST @UPPER@ ***" );

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS     1
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
