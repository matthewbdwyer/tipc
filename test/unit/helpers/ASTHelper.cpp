#include "ASTHelper.h"
#include "ConsoleErrorListener.h"
#include "antlr4-runtime.h"
#include <TIPLexer.h>
#include <TIPParser.h>

std::shared_ptr<ASTProgram> ASTHelper::build_ast(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  TIPParser::ProgramContext *tree = parser.program();
  ASTBuilder tb(&parser);
  return tb.build(tree);
}
