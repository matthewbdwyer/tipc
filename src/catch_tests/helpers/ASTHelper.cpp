#include "ASTHelper.h"
#include <TIPLexer.h>
#include <TIPParser.h>
#include "antlr4-runtime.h"

std::unique_ptr<TIPtree::Program> ASTHelper::build_ast(std::istream &stream) {
  antlr4::ANTLRInputStream input(stream);
  TIPLexer lexer(&input);
  antlr4::CommonTokenStream tokens(&lexer);
  TIPParser parser(&tokens);
  TIPParser::ProgramContext *tree = parser.program();
  TIPtreeBuild tb(&parser);
  return tb.build(tree);
}
