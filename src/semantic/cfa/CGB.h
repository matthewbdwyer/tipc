#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "CFA.h"
#include <map>
#include <ostream>
#include <set>

/*! \class CallGraphBuilder
 *  \brief Generates call graph of a program that represents calling relationships between subroutines
 *  in a computer program. Each node represents a procedure and each edge (a1, a0) indicates that procedure a1 calls procedure a0.
 *  A cycle in the graph indicates recursive procedure calls, e.g., a0 -> a0 indicates that a0 calls itself recursively
 *
 * This class overrides several ASTVisitor's methods
 * printCallGraph prints the call graph of a program on the standard output
 * this call graph is sometimes approximations. Not all the call relationship that exist in the graph will occur in the actual runs of the program.
 */


class CallGraphBuilder : ASTVisitor{
public:
    static std::map<ASTFunction*, std::set<ASTFunction*>> build(ASTProgram*, CFAnalyzer cfa);
    bool visit(ASTFunction * element) override;
    bool visit(ASTFunAppExpr * element) override;
    bool visit(ASTVariableExpr * element) override;
    bool visit(ASTReturnStmt * element) override;
    static int printCallGraph(const std::vector<ASTFunction*>&, const std::map<ASTFunction*, std::set<ASTFunction*>>&, std::ostream&);
private:
    CallGraphBuilder(CFAnalyzer pass);
    ASTNode* getCanonical(ASTNode * n);
    ASTFunction* cfun;
    CFAnalyzer cfa;
    std::map<ASTFunction*, std::set<ASTFunction*>> graph;
};

