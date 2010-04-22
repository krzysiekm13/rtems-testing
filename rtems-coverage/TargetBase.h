/*
 *  $Id$
 */

/*! @file TargetBase.h
 *  @brief TargetBase Specification
 *
 *  This file contains the specification of the TargetBase class.
 */

#ifndef __TARGETBASE_H__
#define __TARGETBASE_H__

#include <list>
#include <string>


namespace Target {

  /*! @class TargetBase
   *
   *  This class is the base class for all Target classes.  Each
   *  target class contains routines that are specific to the target in question.
   *
   */
  class TargetBase {

  public:

    /*!
     *  This method constructs an TargetBase instance.
     */
    TargetBase( std::string targetName );

    /*!
     *  This method destructs an TargetBase instance.
     */
    virtual ~TargetBase();

    /*!
     *  This method determines whether the specified line from a 
     *  objdump file is a nop instruction.
     *
     *  @param[in] line contains the object dump line to check
     *  @param[out] size is set to the size in bytes of the nop
     *
     *  @return Returns TRUE if the instruction is a nop, FALSE otherwise.
     */
    virtual bool isNopLine(
      const char* const line,
      int&              size
    ) = 0;

    /*!
     *  This method determines if the specified line from an
     *  objdump file is a branch instruction.
     */
    virtual bool isBranch(
      const char* const instruction
    ) = 0;

  private:

    /*!
     * This member variable contains the target name string.
     */
    std::string    targetName_m;

  };
}
#endif
