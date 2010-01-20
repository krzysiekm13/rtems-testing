/*
 *  $Id$
 */

/*! @file Toolnames.h
 *  @brief Toolnames Specification
 *
 *  This file contains the specification of the Toolnames class.
 */

#ifndef __TOOLNAMES_H__
#define __TOOLNAMES_H__

#include <stdint.h>
#include <string>

namespace Coverage {

  /*! @class Toolnames
   *
   *  This is the base class for Coverage Map implementations.
   */
  class Toolnames {

  private:

    std::string addr2line;
    std::string objdump;
    std::string target;
    std::string cpu;

  public:

    /*! 
     *  This method is the constructor of a Toolnames instance.
     */
    Toolnames(
      const char *target_arg
    );

    /*! 
     *  This method is the destructor for a Toolnames instance.
     */
    virtual ~Toolnames();

    /*!
     *  This method returns the target name.
     */
    const char *getTarget( void );

    /*!
     *  This method returns the CPU name.
     */
    const char *getCPU( void );

    /*!
     *  This method returns the program name for addr2line.
     */
    const char *getAddr2line( void );

    /*!
     *  This method returns the program name for objdump.
     */
    const char *getObjdump( void );

  };

}
#endif

