#include "CFAnalyzer.h"
#include "loguru.hpp"

CFAnalyzer CFAnalyzer::analyze(ASTProgram* p, SymbolTable* st)
{
    CFAnalyzer cfa(p, st);
    p->accept(&cfa);
    return cfa;
}

std::vector<ASTFunction*> CFAnalyzer::getPossibleFunctionsForExpr(ASTNode* n, ASTFunction* f)
{
    return s.getPossibleFunctionsForExpr(getCanonicalForFunction(n, f));
}

CFAnalyzer::CFAnalyzer(ASTProgram* p, SymbolTable* st): s(p->getFunctions()), symbolTable(st), pgr(p){}

ASTNode* CFAnalyzer::getCanonical(ASTNode* n)
{
    if (auto ve = dynamic_cast<ASTVariableExpr*>(n)) {
        ASTDeclNode* canonical;
        if ((canonical = symbolTable->getLocal(ve->getName(), scope.top()))) {
            return canonical;
        }
        else if ((canonical = symbolTable->getFunction(ve->getName()))) {
            return canonical;
        }
    }  // LCOV_EXCL_LINE
    return n;
}

ASTNode* CFAnalyzer::getCanonicalForFunction(ASTNode* n, ASTFunction* scp)
{
    if (auto ve = dynamic_cast<ASTVariableExpr*>(n)) {
        ASTDeclNode* canonical;
        if ((canonical = symbolTable->getLocal(ve->getName(), scp->getDecl()))) {
            return canonical;
        }
        else if ((canonical = symbolTable->getFunction(ve->getName()))) {
            return canonical;
        }
    }  // LCOV_EXCL_LINE
    return n;
}

bool CFAnalyzer::visit(ASTFunction* element)
{
    scope.push(element->getDecl());
    s.addElementofConstraint(element, getCanonical(element->getDecl()));
    return true;
}
void CFAnalyzer::endVisit(ASTFunction* element)
{
    scope.pop();
}
bool CFAnalyzer::visit(ASTFunAppExpr* element)
{
    for (ASTFunction* fun : pgr->getFunctions()) {
        if (fun->getFormals().size() == element->getActuals().size()) {
            for (int i = 0; i < fun->getFormals().size(); i++) {
                s.addConditionalConstraint(fun, getCanonical(element->getFunction()), getCanonical(element->getActuals()[i]), getCanonicalForFunction(fun->getFormals()[i], fun));
                auto stmts = fun->getStmts();
                ASTReturnStmt* ret;
                if (!(ret = dynamic_cast<ASTReturnStmt*>(stmts[stmts.size() - 1]))) {
                    assert(false); // LCOV_EXCL_LINE
                }
                s.addConditionalConstraint(fun, getCanonical(element->getFunction()), getCanonicalForFunction(ret->getArg(), fun), getCanonical(element));
            }
        }
    }
    return true;
}  // LCOV_EXCL_LINE

bool CFAnalyzer::visit(ASTAssignStmt* element)
{
    s.addSubseteqConstraint(getCanonical(element->getRHS()), getCanonical(element->getLHS()));
    return true;
}
