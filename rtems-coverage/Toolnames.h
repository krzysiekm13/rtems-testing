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
   *  This class provides various target specific toolnames for use
   *  during analysis.
   */
  class Toolnames {

  public:

    /*! 
     *  This method constructs a Toolnames instance.
     *
     *  @param[in] target_arg specifies the desired target
     */
    Toolnames(
      const char* const target_arg
    );

    /*! 
     *  This method destructs a Toolnames instance.
     */
    virtual ~Toolnames();

    /*!
     *  This method returns the program name for addr2line.
     *
     *  @return Returns the target specific addr2line program name
     */
    const char* getAddr2line( void ) const;

    /*!
     *  This method returns the CPU name.
     *
     *  @return Returns the target cpu name
     */
    const char* getCPU( void ) const;

    /*!
     *  This method returns the program name for nm.
     *
     *  @return Returns the target specific nm program name
     */
    const char* getNm( void ) const;

    /*!
     *  This method returns the program name for objdump.
     *
     *  @return Returns the target specific objdump program name
     */
    const char* getObjdump( void ) const;

    /*!
     *  This method returns the target name.
     *
     *  @return Returns the target name
     */
    const char* getTarget( void ) const;

  private:

    std::string addr2line;
    std::string cpu;
    std::string nm;
    std::string objdump;
    std::string target;

  };

}
#endif
