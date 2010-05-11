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
#include <stdint.h>

namespace Target {

  /*! @class TargetBase
   *
   *  This class is the base class for all Target classes.  Each
   *  target class contains routines that are specific to the target
   *  in question.
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
     *  This method determins if the objdump line contains a
     *  branch instruction.
     *
     *  @param[in] line contains the object dump line to check
     *  @param[out] size is set to the size in bytes of the nop
     *
     *  @return Returns TRUE if the instruction is a branch, FALSE otherwise.
     */
    bool isBranchLine(
      const char* const line
    );

    
    /*!
     *  This method determines if the specified line from an
     *  objdump file is a branch instruction.
     */
    bool isBranch(
      const char* const instruction
    );

    /*!
     *  This method returns the bit set by Qemu in the trace record
     *  when a branch is taken.
     */
    virtual uint8_t qemuTakenBit(void);

    /*!
     *  This method returns the bit set by Qemu in the trace record
     *  when a branch is taken.
     */
    virtual uint8_t qemuNotTakenBit(void);

  protected:

    /*!
     * This member variable contains the target name string.
     */
    std::string    targetName_m;

    /*!
     * This member variable indicates either the column that the instruction
     * starts in the object dump file, when the objdump has no tabs; or the
     * number of tabs to find the instruction.
     */
    int objdumpInstructionLocation;

    /*!
     *  This member variable indicates whether or not the objdump has
     *  tabs as delemeters.
     */
    bool objdumpHasTabs;

    /*!
     * This member variable is an array of all branch instructions
     * for this target.
     */
    std::list <std::string> branchInstructions;
  };
}
#endif
