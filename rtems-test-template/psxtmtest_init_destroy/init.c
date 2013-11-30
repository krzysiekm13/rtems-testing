/*
 *  COPYRIGHT (c) 1989-2013.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <timesys.h>
#include <rtems/timerdrv.h>
#include "test_support.h"

/* forward declarations to avoid warnings */
void *POSIX_Init(void *argument);
void benchmark_NAME_OF_CREATE_OPERATION(void);
void benchmark_NAME_OF_DELETE_OPERATION(void);

void benchmark_NAME_OF_CREATE_OPERATION(void)
{
  long end_time;
  int  status;

  benchmark_timer_initialize();
    /* XXX single shot CREATE operation goes here -- test expected status */
    /* XXX outside of the benchmark'ed region */
  end_time = benchmark_timer_read();
  rtems_test_assert( status == 0 );

  put_time(
    "@DESC@", /* XXX fix to indicate create name */
    end_time,
    1,        /* Only executed once */
    0,
    0
  );

}

void benchmark_NAME_OF_DELETE_OPERATION(void)
{
  long end_time;
  int  status;

  benchmark_timer_initialize();
    /* XXX single shot operation goes here -- test expected status outside */
    /* XXX of the benchmark'ed region */
  end_time = benchmark_timer_read();
  rtems_test_assert( status == 0 );

  put_time(
    "@DESC@", /* XXX fix to indicate delete name */
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

  benchmark_NAME_OF_CREATE_OPERATION();
  benchmark_NAME_OF_DELETE_OPERATION();

  puts( "*** END OF POSIX TIME TEST @UPPER@ ***" );
  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER

/* XXX configure an instance of the object created/destroyed */

#define CONFIGURE_MAXIMUM_POSIX_THREADS     1
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
/* end of file */
