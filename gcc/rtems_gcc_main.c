/*
 *  Main for GCC Tests
 *  
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

#include <bsp.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Set up first argument
 */
static int   argc     = 1;
static char  arg0[20] = "rtems";
static char *argv[20] = { arg0 };

int main(int argc, char **argv, char **environp);

rtems_task Init(rtems_task_argument ignored)
{
  mkdir( "/tmp", 0777 );
  main(argc, argv, NULL);
}

/* configuration information */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20

/* might as well treat all memory as one pool */
#define CONFIGURE_UNIFIED_WORK_AREAS
#define CONFIGURE_UNLIMITED_OBJECTS

/* GCC tests start at main, use a lot of stack and may use the FPU */
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE
#if defined(__m32c__)
  #define CONFIGURE_INIT_TASK_STACK_SIZE    (16 * 1024)
#else
  #define CONFIGURE_INIT_TASK_STACK_SIZE    (256 * 1024)
#endif
#define CONFIGURE_INIT_TASK_ATTRIBUTES    RTEMS_FLOATING_POINT

/* This helps language tests which need a real filesystem */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20

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
