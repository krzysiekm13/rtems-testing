/*
 *  $Id$
 */

/*! @file SymbolTable.h
 *  @brief SymbolTable Specification
 *
 *  This file contains the specification of the SymbolTable class.
 */

#ifndef __SYMBOLTABLE_H__
#define __SYMBOLTABLE_H__

#include <stdint.h>
#include <string>
#include <map>

namespace Coverage {

  /*! @class SymbolTable
   *
   *  This class maintains information for each desired symbol within an
   *  executable.  A desired symbol is a symbol for which analysis is to
   *  be performed.
   */
  class SymbolTable {

  public:

    /*!
     *  This structure defines the information kept for each symbol.
     */
    typedef struct {
      uint32_t startingAddress;
      uint32_t length;
    } symbolInfo;

    /*!
     *  This method constructs a SymbolTable instance.
     */
    SymbolTable();

    /*!
     *  This method destructs a SymbolTable instance.
     */
    virtual ~SymbolTable();

    /*!
     *  This method returns the symbol information for the specified symbol.
     *
     *  @param[in] symbol specifies the symbol for which to obtain information
     *
     *  @return Returns a pointer to the symbol information
     */
    symbolInfo* getInfo(
      const std::string& symbol
    );

    /*!
     *  This method returns the length in bytes of the specified symbol.
     *
     *  @param[in] symbol specifies the symbol for which to obtain the length
     *
     *  @return Returns the length of the symbol
     */
    uint32_t getLength(
      const std::string& symbol
    );

    /*!
     *  This method returns the symbol that contains the specified address.
     *
     *  @param[in] address specifies the address for which to obtain the symbol
     *
     *  @return Returns the symbol containing the address
     */
    std::string getSymbol(
      uint32_t address
    );

    /*!
     *  This method loads the symbol table from the specified executable.
     *
     *  @param[in] executableName specifies the file name of the executable
     *
     *  @return Returns TRUE if the symbol table was loaded and
     *          FALSE otherwise.
     */
    bool load(
      const std::string& executableName
    );

  private:

    /*!
     *  This map associates each address from an executable
     *  with the symbol that contains the address.
     */
    typedef struct {
       uint32_t min;
       uint32_t max;
       std::string symbol;
    } symbol_entry_t;
    typedef std::map< uint32_t, symbol_entry_t > contents_t;
    contents_t contents;


    /*!
     *  This map associates each symbol from an executable with
     *  the symbol's information.
     */
    typedef std::map<std::string, symbolInfo> info_t;
    info_t info;

  };
}
#endif
