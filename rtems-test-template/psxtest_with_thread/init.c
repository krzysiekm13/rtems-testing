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

#include <tmacros.h>
#include "test_support.h"
#include <pthread.h>

pthread_t ThreadId;

void *TestThread(
  void *argument
)
{
  return NULL;
}

void *POSIX_Init(
  rtems_task_argument argument
)
{
  int status;

  puts( "\n\n*** POSIX TEST @DESC@ ***" );

  status = pthread_create( &ThreadId, NULL, TestThread, NULL );
  rtems_test_assert( !status );

  puts( "*** END OF POSIX TEST @DESC@ ***" );

  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MAXIMUM_POSIX_THREADS        2

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */
