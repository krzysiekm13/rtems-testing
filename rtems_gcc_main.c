/*  Init
 *
 *  COPYRIGHT (c) 1989-2008.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include <rtems.h>

int main( int, char **, char **);

/* configuration information */
#define CONFIGURE_MAXIMUM_TASKS 1
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20

/* GCC tests start at main, use a lot of stack and may use the FPU */
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#define CONFIGURE_INIT_TASK_ENTRY_POINT   (void *)main
#define CONFIGURE_INIT_TASK_STACK_SIZE    (256 * 1024)
#define CONFIGURE_INIT_TASK_ATTRIBUTES    RTEMS_FLOATING_POINT

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_INIT

#include <rtems/confdefs.h>

/* global variables */

#if defined(__h8300__)
void _wrap__exit(int e)
{
  return _wrap___exit(e);
}
#endif

#if defined(__mips__)
/* GCC sometimes expects this on the mips */
void _flush_cache()
{
}
#endif
