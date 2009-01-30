/* 
 *  COPYRIGHT (c) 1989-1997.
 *  On-Line Applications Research Corporation (OAR).
 *  Copyright assigned to U.S. Government, 1994.
 *
 *  The license and distribution terms for this file may be found in
 *  the file LICENSE in this distribution or at
 *  http://www.OARcorp.com/rtems/license.html.
 *
 *  $Id$
 */

#include <bsp.h>

#include <assert.h>
#include <pthread.h>
#include <stdlib.h>

#include <sys/stat.h>
#include <sys/types.h>

extern rtems_configuration_table  BSP_Configuration;

#ifdef GNAT_PID
#include <unistd.h>
pid_t getpid()
{
  return GNAT_PID;
}
#endif

/*
 *  By having the POSIX_Init thread create a second thread just
 *  to invoke gnat_main, we can override all default attributes
 *  of the "Ada environment task".  Otherwise, we would be
 *  stuck with the defaults set by RTEMS.
 */
 
void *start_gnat_main( void * argument )
{
  extern int gnat_main ( int argc, char **argv, char **envp );

  (void) gnat_main ( 0, 0, 0 );

  exit( 0 );

  return 0;
}

size_t  _ada_pthread_minimum_stack_size()
{
  return 20 * 1024;
}

void *POSIX_Init( void *argument )
{
  pthread_t       thread_id;
  pthread_attr_t  attr;
  size_t          stacksize = _ada_pthread_minimum_stack_size();
  int             status;

  /* ACATS needs /tmp */
  mkdir( "/tmp", 0777 );

  status = pthread_attr_init( &attr );
  assert( !status );

  #ifndef GNAT_MAIN_STACKSPACE
    #define GNAT_MAIN_STACKSPACE 100
  #endif

  #ifdef GNAT_MAIN_STACKSPACE
    stacksize = GNAT_MAIN_STACKSPACE * 1024;
  #else
    #define GNAT_MAIN_STACKSPACE 0
  #endif

  status = pthread_attr_setstacksize( &attr, stacksize );
  assert( !status );

  status = pthread_create( &thread_id, &attr, start_gnat_main, NULL );
  assert( !status );

  pthread_exit( 0 );

  return 0;
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

#define CONFIGURE_MICROSECONDS_PER_TICK RTEMS_MILLISECONDS_TO_MICROSECONDS(10)

#define CONFIGURE_POSIX_INIT_THREAD_TABLE

#define CONFIGURE_MAXIMUM_SEMAPHORES 10
#define CONFIGURE_GNAT_RTEMS
#define CONFIGURE_MAXIMUM_ADA_TASKS      40
#define CONFIGURE_MAXIMUM_FAKE_ADA_TASKS 0

/* Some of the ACATS tests assume a real filesystem */
#define CONFIGURE_USE_IMFS_AS_BASE_FILESYSTEM
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 20

#define CONFIGURE_MEMORY_OVERHEAD        (GNAT_MAIN_STACKSPACE)
/*
#define CONFIGURE_MEMORY_OVERHEAD        (256 + GNAT_MAIN_STACKSPACE)
*/

#define CONFIGURE_ZERO_WORKSPACE_AUTOMATICALLY 1
/* #define CONFIGURE_MALLOC_DIRTY */
#define STACK_CHECKER_ON
#define CONFIGURE_INIT

#include <rtems/confdefs.h>

#if defined(__mips__)
/* GCC sometimes expects this on the mips */
void _flush_cache()
{
}
#endif
