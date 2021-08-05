#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "CFAnalyzer.h"
#include <map>
#include <ostream>
#include <set>

/*! \class CallGraphBuilder
 *  \brief Builds call graph of a program that represents calling relationships between subroutines
 * This class overrides several ASTVisitor's methods to populate the call graph's vertices and edges. Each vertex is a subroutine
 * of the given program, and each edge represents which subroutine calls which subroutines
 */


class CallGraphBuilder : ASTVisitor{
public:

    /*! \brief Return the call graph of a given program.
    * \param ast The AST of the program
    * \param cfa The control flow analyzer
    * \return the call graph for the given program
    */
    static CallGraphBuilder build(ASTProgram* ast, CFAnalyzer cfa);
    bool visit(ASTFunction * element) override;
    bool visit(ASTFunAppExpr * element) override;
    bool visit(ASTVariableExpr * element) override;
    bool visit(ASTReturnStmt * element) override;
    std::map<ASTFunction*, std::set<ASTFunction*>> getCallGraph();
    std::map<std::string, ASTFunction*> getFunMap();

private:
    CallGraphBuilder(CFAnalyzer pass);
    ASTNode* getCanonical(ASTNode * n);
    ASTFunction* cfun;
    CFAnalyzer cfa;
    std::map<ASTFunction*, std::set<ASTFunction*>> graph;
    std::map<std::string, ASTFunction*> fromFunNameToASTFun;

};

