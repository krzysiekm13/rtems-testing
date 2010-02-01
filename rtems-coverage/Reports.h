/*
 *   $Id$
 */

#ifndef __REPORTS_H__
#define __REPORTS_H__

#include <stdint.h>

/*
 *  Write annotated report
 */
void WriteAnnotatedReport(
  const char *fileName,
  uint32_t    lowAddress,
  uint32_t    highAddress
);

/*
 *  Write branch report
 */
void WriteBranchReport(
  const char *fileName,
  uint32_t    lowAddress,
  uint32_t    highAddress
);

/*
 *  Write coverage report
 */
void WriteCoverageReport(
  const char *fileName
);

/*
 *  Write size report
 */
void WriteSizeReport(
  const char *fileName
);

#endif
