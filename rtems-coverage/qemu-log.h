/*
 *  $Id$
 */


/*
 * Qemu log file format.
 */

#ifndef QEMU_LOG_H
#define QEMU_LOG_H

#define QEMU_LOG_SECTION_END    "----------------"
#define QEMU_LOG_IN_KEY         "IN: "

typedef struct {
  unsigned long address;
  char          instruction[10];
  char          data[20];
}  QEMU_LOG_IN_Block_t;


#endif /* QEMU_LOG_H */
