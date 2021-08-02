


#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "CFACubicSolver.h"
#include <stack>
#include "SymbolTable.h"


/*! \class CFAnalyzer
 * \brief Performs control flow analyses with the help of AST and Symbol table of a program
 * Provides helper functions to support call graph generation,
 * Overrides several ASTVisitor's methods that visit ASTFunction, ASTFunAppExpr and ASTAssignStmt nodes to generate
 * constraints
 * Generated constraints are solved by cubic solver
 */

class CFAnalyzer : ASTVisitor{
public:
    static CFAnalyzer analyze(ASTProgram*, SymbolTable* st);
    bool visit(ASTFunction * element) override;
    bool visit(ASTFunAppExpr * element) override;
    bool visit(ASTAssignStmt * element) override;
    void endVisit(ASTFunction * element) override;
    std::vector<ASTFunction*> getPossibleFunctionsForExpr(ASTNode* n, ASTFunction* f);
private:
    CFAnalyzer(ASTProgram*, SymbolTable* st);
    ASTNode* getCanonical(ASTNode * n);
    ASTNode* getCanonicalForFunction(ASTNode * n, ASTFunction*);
    CubicSolver s;
    std::stack<ASTDeclNode *> scope;
    SymbolTable* symbolTable;
    ASTProgram* pgr;
};