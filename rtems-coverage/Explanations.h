/*
 *  $Id$
 */

/*! @file Explanations.h
 *  @brief Explanations Specification
 *
 *  This file contains the specification of the Explanations class.
 */

#ifndef __EXPLANATIONS_H__
#define __EXPLANATIONS_H__

#include <stdint.h>
#include <map>
#include <string>

namespace Coverage {

  class Explanation {
  public:

    std::string startingPoint;
    std::string explanation;
    std::string classification;
    bool        found;

    Explanation() {found = false;}

    ~Explanation() {}
  };

  /*! @class Explanations
   *
   *  This is the base class for Coverage Map implementations.
   */
  class Explanations {

  public:
    /*!
     */
    std::map<std::string, Explanation> Set;

    /*! 
     *  This method is the default constructor of a Explanations instance.
     */
    Explanations();

    /*! 
     *  This method is the destructor for a Explanations instance.
     */
    ~Explanations();

    /*!
     */
    void load(
      const char *explanations
    );

    /*!
     */
    std::string lookupClassification(
      std::string start
    );

    /*!
     */
    std::string lookupExplanation(
      std::string start
    );

    /*!
     */  
    void writeNotFound(
      const char *fileName
    );

  protected:

    bool NotFoundOccurred;
  };

}
#endif

