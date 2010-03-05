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

#include <list>
#include <string>

#include "ExecutableInfo.h"

namespace Coverage {

  /*! @class ObjdumpProcessor
   *
   *  This class implements the functionality which reads the output of
   *  an objdump.  Various information is extracted from the objdump line
   *  to support analysis and report writing.  Analysis of the objdump line
   *  also allows for identification of "nops".  For the purpose of coverage
   *  analysis, nops in the executable may be ignored.  Compilers often
   *  produce nops to align functions on particular alignment boundaries
   *  and the nop between functions can not possibly be executed.
   */
  class ObjdumpProcessor {

  public:

    /*!
     *  This type defines the elements of an objdump line.
     */
    typedef struct {
      /*!
       *  This member variable contains the actual line from the object dump.
       */
      std::string line;

      /*!
       *  This member variable contains the address from the object dump line.
       */
      uint32_t address;

      /*!
       *  This member variable contains an indication of whether the line
       *  is an instruction.
       */
      bool isInstruction;

      /*!
       *  This member variable contains an indication of whether the line
       *  is a nop instruction.
       */
      bool isNop;

      /*!
       *  This member variable contains the size of the nop instruction.
       */
      int nopSize;

    } objdumpLine_t;

    /*!
     *  This method constructs an ObjdumpProcessor instance.
     */
    ObjdumpProcessor();

    /*!
     *  This method destructs an ObjdumpProcessor instance.
     */
    virtual ~ObjdumpProcessor();

    /*!
     *  This method generates and processes an object dump for
     *  the specified executable.
     */
    void load(
      ExecutableInfo* const executableInformation
    );

  private:

    /*!
     *  This method determines whether the specified line is a
     *  nop instruction.
     *
     *  @param[in] line contains the object dump line to check
     *  @param[out] size is set to the size in bytes of the nop
     *
     *  @return Returns TRUE if the instruction is a nop, FALSE otherwise.
     */
    bool isNop(
      const char* const line,
      int&              size
    );

  };
}
#endif
