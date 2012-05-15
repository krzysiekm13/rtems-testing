/*
 *  COPYRIGHT (c) 1989-2011.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 */

/* XXX these will have to be tailored */
#define FATAL_ERROR_TEST_NAME            "@DESC@"
#define FATAL_ERROR_DESCRIPTION          "@DESC@"
#define FATAL_ERROR_EXPECTED_SOURCE      INTERNAL_ERROR_RTEMS_API
#define FATAL_ERROR_EXPECTED_IS_INTERNAL FALSE
#define FATAL_ERROR_EXPECTED_ERROR       0

void force_error()
{
  /* XXX if not triggered directly or indirectly via a configuration */
  /* XXX error, then do something here to trigger it */

  /* we will not run this far */
}
