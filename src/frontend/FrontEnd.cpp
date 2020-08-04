#include "FrontEnd.h"

#include "antlr4-runtime.h"
#include "TIPLexer.h"
#include "TIPParser.h"
#include "ASTBuilder.h"
#include "PrettyPrinter.h"
#include "ParseError.h"

using namespace std;
using namespace antlr4;

//! Lexer and Parser Error Listeners for redirecting ANTLR4 errors to ParseError.
class LexerErrorListener : public BaseErrorListener {
public:
  LexerErrorListener() = default;

  virtual void syntaxError(Recognizer *recognizer, Token *offendingSymbol, 
                           size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override;
};

class ParserErrorListener : public BaseErrorListener {
public:
  ParserErrorListener() = default;

  virtual void syntaxError(Recognizer *recognizer, Token *offendingSymbol, 
                           size_t line, size_t charPositionInLine,
                           const std::string &msg, std::exception_ptr e) override;
};

void LexerErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, 
                                     size_t line, size_t charPositionInLine, 
                                     const std::string &msg, std::exception_ptr e) {
  throw ParseError(msg);
}

void ParserErrorListener::syntaxError(Recognizer *recognizer, Token *offendingSymbol, 
                                     size_t line, size_t charPositionInLine, 
                                     const std::string &msg, std::exception_ptr e) {
  throw ParseError(msg);
}


std::unique_ptr<ASTProgram> FrontEnd::parse(std::ifstream& stream){
  ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  LexerErrorListener lexerErrorListener;
  ParserErrorListener parserErrorListener;

  // Add error listeners
  lexer.removeErrorListeners();
  lexer.addErrorListener(&lexerErrorListener);

  parser.removeParseListeners();
  parser.removeErrorListeners();
  parser.addErrorListener(&parserErrorListener);

  TIPParser::ProgramContext *tree = parser.program();

  ASTBuilder ab(&parser);
  return ab.build(tree);
}

void FrontEnd::prettyprint(ASTProgram* program, std::ostream& os) {
  PrettyPrinter::print(program, os, ' ', 2);
}
