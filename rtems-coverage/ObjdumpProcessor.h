/*
 *  $Id$
 */

/*! @file ObjdumpProcessor.h
 *  @brief ObjdumpProcessor Specification
 *
 *  This file contains the specification of the ObjdumpProcessor class.
 */

#ifndef __OBJDUMP_PROCESSOR_H__
#define __OBJDUMP_PROCESSOR_H__

#include "CoverageReaderBase.h"
#include <string>
#include <list>

namespace Coverage {

  class ObjdumpLine;

  /*! @class ObjdumpProcessor
   *
   *  This class implements the class which reads an objdump output
   *  file and looks for "nops."  For the purpose of coverage
   *  analysis, nops in the executable may be ignored.  The compiler
   *  may produce nops to align functions on particular alignment
   *  boundaries and the nop between functions can not possibly be
   *  executed.
   */
  class ObjdumpProcessor {
  private:

     std::list<ObjdumpLine> Contents;

     bool isInstruction(
       const char *line
     );
     bool isNop(
       const char *line,
       int        &size
     );

  public:

    ObjdumpProcessor();

    virtual ~ObjdumpProcessor();

    bool initialize(
      const char      *executable,
      CoverageMapBase *coverage
    );

    bool markNopsAsExecuted(
      CoverageMapBase *coverage
    );

    bool writeAnnotated(
      CoverageMapBase *coverage,
      uint32_t         low,
      uint32_t         high,
      const char      *annotatedText,
      const char      *annotatedHTML
    );
  };

}
#endif

