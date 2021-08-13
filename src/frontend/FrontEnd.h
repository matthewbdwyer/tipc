#pragma once

#include "ASTProgram.h"

#include "antlr4-runtime.h"

#include <iostream>
#include <fstream>

//! \brief Lexer error listener for redirecting ANTLR4 errors to ParseError.
class LexerErrorListener : public antlr4::BaseErrorListener {
public:
  LexerErrorListener() = default;

  virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                           size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override;
};

//! \brief Parse error listener for redirecting ANTLR4 errors to ParseError.
class ParserErrorListener : public antlr4::BaseErrorListener {
public:
  ParserErrorListener() = default;

  virtual void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                           size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override;
};


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
  static std::unique_ptr<ASTProgram> parse(std::istream& stream);

  /*! \fn print
   *  \brief Print program in a standard form to cout.
   *
   * \param program the root of an AST for a TIP program.
   */ 
  static void prettyprint(ASTProgram* program, std::ostream& os);
};
