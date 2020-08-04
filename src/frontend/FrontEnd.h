#pragma once

#include "ASTProgram.h"
#include <iostream>
#include <fstream>

/*! \class FrontEnd
 *  \brief A collection of routines implementing the compiler front end.
 */
class FrontEnd {
public:
  /*! \fn parse
   *  \brief Parse an input stream and return an AST.
   *
   * Parsing can detect errors, which are reported via throw of a ParseError
   * exception.  In the absence of errors, ownership of the generated AST is 
   * transfered to the caller.
   * \param stream the input stream holding the program text.
   * \return the generated AST.
   */
  static std::unique_ptr<ASTProgram> parse(std::ifstream& stream);

  /*! \fn print
   *  \brief Print program in a standard form to cout.
   *
   * \param program the root of an AST for a TIP program.
   */ 
  static void prettyprint(ASTProgram* program, std::ostream& os);
};
