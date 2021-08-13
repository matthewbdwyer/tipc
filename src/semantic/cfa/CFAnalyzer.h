#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "CubicSolver.h"
#include <stack>
#include "SymbolTable.h"

/*! \class CFAnalyzer
 * \brief Performs control flow analyses with the help of AST and Symbol table of a program
 * Provides helper functions to support call graph generation,
 * Overrides several ASTVisitor's methods that visit ASTFunction, ASTFunAppExpr, and ASTAssignStmt nodes to generate
 * constraints
 * Generated constraints are solved by the cubic solver
 */

class CFAnalyzer : ASTVisitor {
public:
    /*! \brief analyzes the AST and symbol table for a given program. Generates control flow constraints.
     * \param The AST of the program
     * \param st The symbol table of a given program
     * \return the CFAnalyzer for subsequent use for the CallGraphBuilder
     */

    static CFAnalyzer analyze(ASTProgram* p, SymbolTable* st);
    bool visit(ASTFunction* element) override;
    bool visit(ASTFunAppExpr* element) override;
    bool visit(ASTAssignStmt* element) override;
    void endVisit(ASTFunction* element) override;
    std::vector<ASTFunction*> getPossibleFunctionsForExpr(ASTNode* n, ASTFunction* f);

private:
    CFAnalyzer(ASTProgram* p, SymbolTable* st);
    ASTNode* getCanonical(ASTNode* n);
    ASTNode* getCanonicalForFunction(ASTNode* n, ASTFunction*);
    CubicSolver s;
    std::stack<ASTDeclNode*> scope;
    SymbolTable* symbolTable;
    ASTProgram* pgr;
};