#include "ASTHelper.h"
#include <TIPLexer.h>
#include <TIPParser.h>
#include "antlr4-runtime.h"

std::unique_ptr<AST::Program> ASTHelper::build_ast(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  TIPParser::ProgramContext *tree = parser.program();
  ASTBuilder tb(&parser);
  return tb.build(tree);
}

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

bool ASTHelper::check_parse(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);

  std::shared_ptr<bool> parseError = std::make_shared<bool>(false);
  ErrorListener errorListener(parseError);

  // Add error listeners
  lexer.addErrorListener(&errorListener);
  parser.addErrorListener(&errorListener);

  TIPParser::ProgramContext *tree = parser.program();
  return *parseError;
}

