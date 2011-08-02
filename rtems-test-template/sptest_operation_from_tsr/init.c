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

volatile bool operation_performed_from_tsr;

rtems_timer_service_routine test_operation_from_isr(
  rtems_id  timer,
  void     *arg
)
{
  /* do something from ISR */

  operation_performed_from_tsr = true;
}

rtems_task Init(
  rtems_task_argument argument
)
{
  rtems_status_code     status;
  rtems_id              timer;

  puts( "\n\n*** TEST @UPPER@ ***" );

  /*
   *  Timer used in multiple ways
   */
  status = rtems_timer_create( rtems_build_name('T', 'M', 'R', '0'), &timer );
  directive_failed( status, "rtems_timer_create" );

  operation_performed_from_tsr = false;

  /*
   * Test Operation from ISR
   */
  status = rtems_timer_fire_after( timer, 10, test_operation_from_isr, NULL );
  directive_failed( status, "timer_fire_after failed" );

  /* XXX pick a delay method */
#if 0
  status = rtems_task_wake_after( 20 );
#else
  {
    rtems_interval        start;
    rtems_interval        now;
    start = rtems_clock_get_ticks_since_boot();
    do {
      now = rtems_clock_get_ticks_since_boot();
    } while ( (now-start) < 100 );
  }
#endif
  if ( !operation_performed_from_tsr ) {
    puts( "Operation from ISR did not get processed\n" );
    rtems_test_exit( 0 );
  }

  /* XXX also may be able to confirm operation actually was performed */

  puts( "Operation from ISR successfully processed" );
  puts( "*** END OF TEST @UPPER@ ***" );
  rtems_test_exit( 0 );
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_MAXIMUM_TASKS             1
#define CONFIGURE_MAXIMUM_TIMERS            1

#define CONFIGURE_INIT
#include <rtems/confdefs.h>
/* end of file */

