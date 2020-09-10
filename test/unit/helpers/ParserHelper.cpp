#include "ParserHelper.h"
#include <TIPLexer.h>
#include <TIPParser.h>
#include "antlr4-runtime.h"
#include "ConsoleErrorListener.h"

// Handle parse errors
class ErrorListener : public antlr4::BaseErrorListener {
  std::shared_ptr<bool> error;
public:
  ErrorListener(std::shared_ptr<bool> e) : error(e) {}

  void syntaxError(antlr4::Recognizer *recognizer, antlr4::Token *offendingSymbol,
                   size_t line, size_t charPositionInLine,
                   const std::string &msg, std::exception_ptr e) {
    *error = true;
  }
};

bool ParserHelper::is_parsable(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);

  std::shared_ptr<bool> parseError = std::make_shared<bool>(false);
  ErrorListener errorListener(parseError);

  // Set error listeners
  lexer.addErrorListener(&errorListener);
  lexer.removeErrorListener(&antlr4::ConsoleErrorListener::INSTANCE);
  parser.addErrorListener(&errorListener);
  parser.removeErrorListener(&antlr4::ConsoleErrorListener::INSTANCE);

  TIPParser::ProgramContext *tree = parser.program();
  return !*parseError;
}

/* Assumes ParserHelper::is_parseable() is TRUE */
std::string ParserHelper::parsetree(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  TIPParser::ProgramContext *tree = parser.program();
  // Return printed string
  return tree->toStringTree(&parser,false);
}

