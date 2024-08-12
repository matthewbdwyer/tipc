#include "ASTHelper.h"
#include "ASTVisitor.h"
#include "ConsoleErrorListener.h"
#include "antlr4-runtime.h"
#include <TIPLexer.h>
#include <TIPParser.h>

std::shared_ptr<ASTProgram> ASTHelper::build_ast(std::stringstream &stream) {
    antlr4::ANTLRInputStream input(stream);
    TIPLexer lexer(&input);
    antlr4::CommonTokenStream tokens(&lexer);
    TIPParser parser(&tokens);
    TIPParser::ProgramContext *tree = parser.program();
    ASTBuilder tb(&parser);
    return tb.build(tree);
}

template<class N>
class NodeFinder : public ASTVisitor {
public:
    static std::shared_ptr<N> find_node(ASTProgram *p) {
        NodeFinder visitor;
        p->accept(&visitor);
        return visitor.found_node;
    }

    virtual void endVisit(N *element) {  found_node = element; };

private:
    static std::shared_ptr<N> found_node = nullptr;
};

template<class N> std::shared_ptr<N> ASTHelper::find_node(std::shared_ptr<ASTProgram> p) {
    return NodeFinder<N>::find_node(p);
}

