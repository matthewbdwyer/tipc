#include "ASTHelper.h"
#include <TIPLexer.h>
#include <TIPParser.h>
#include "antlr4-runtime.h"
#include "ConsoleErrorListener.h"

std::unique_ptr<ASTProgram> ASTHelper::build_ast(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  TIPParser::ProgramContext *tree = parser.program();
  ASTBuilder tb(&parser);
  return tb.build(tree);
}

