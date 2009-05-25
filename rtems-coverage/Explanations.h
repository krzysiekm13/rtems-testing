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
#include <vector>

namespace Coverage {

  class Explanation {
  public:

    /*!
     *  This is the starting line number of the uncovered range.
     */
    std::string              startingPoint;

    /*!
     *  This is the classification of the range.
     */
    std::string              classification;

    /*!
     *  This is the multi-line explanation.
     */
    std::vector<std::string> explanation;

    /*!
     *  This indicates whether we used this explanation on this run.
     */
    bool                     found;

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
    const Explanation *lookupExplanation(
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

