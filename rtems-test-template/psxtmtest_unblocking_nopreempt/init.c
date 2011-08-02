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

#include <coverhd.h>
#include <tmacros.h>
#include <timesys.h>
#include "test_support.h"
#include <pthread.h>
#include <sched.h>
#include <rtems/timerdrv.h>

pthread_mutex_t MutexId;

void *Blocker(
  void *argument
)
{
  (void) /* XXX blocking operation goes here */
  /* should never return */
  rtems_test_assert( FALSE );

  return NULL;
}

void *POSIX_Init(
  void *argument
)
{
  int        status;
  pthread_t  threadId;
  long       end_time;

  puts( "\n\n*** POSIX TIME TEST @UPPER@ ***" );

  status = pthread_create( &threadId, NULL, Blocker, NULL );
  rtems_test_assert( status == 0 );

  /*
   * Deliberately create the XXX after the threads.  This way if the
   * threads do run before we intend, they will get an error.
   */
  status = 0; /* XXX create resource */
  rtems_test_assert( status == 0 );

  /*
   * Ensure the mutex is unavailable so the other threads block.
   */
  status = 0; /* XXX lock resource to ensure thread blocks */
  rtems_test_assert( status == 0 );

  /*
   * Let the other thread start so the thread startup overhead,
   * is accounted for.  When we return, we can start the benchmark.
   */
  sched_yield();
    /* let other thread run */

  benchmark_timer_initialize();
    status = 0; /* XXX unblocking operation goes here */
  end_time = benchmark_timer_read();
  rtems_test_assert( status == 0 );

  put_time(
    "@DESC",
    end_time,
    1,
    0,
    0
  );

  puts( "*** END OF POSIX TIME TEST @UPPER@ ***" );
  rtems_test_exit( 0 );

  return NULL;
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_TIMER_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS     2
/* XXX make sure you configure the resource */
#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT

#include <rtems/confdefs.h>
  /* end of file */
