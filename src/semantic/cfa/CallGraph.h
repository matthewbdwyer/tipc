#pragma once

#include <map>
#include <vector>
#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "SymbolTable.h"
#include "CallGraphBuilder.h"
#include <set>

/*! \class CallGraph
 *  \brief Performs control flow analysis and records results for subsequent phases
 *  Generates call graph of a program that represents calling relationships between subroutines
 *  in a computer program. Each node represents a subroutine and each edge (a1, a0) indicates that procedure a1 calls procedure a0.
 *  A cycle in the graph indicates recursive procedure calls, e.g., a0 -> a0 indicates that a0 calls itself recursively
 *  this call graph is sometimes approximations. Not all the call relationship that exist in the graph will occur in the actual runs of the program.
 */

class CallGraph {

    std::vector<ASTFunction*> vertices;
    std::vector<std::pair<ASTFunction*, ASTFunction*> > edges;
    int total_edges;
    int total_vertices;
    std::map<ASTFunction*, std::set<ASTFunction*> > callGraph;
    std::map<std::string, ASTFunction*> fromFunNameToASTFuns;



public:

    CallGraph(std::map<ASTFunction*, std::set<ASTFunction*> > cGraph, std::vector<ASTFunction*> funs, std::map<std::string, ASTFunction*> fmap)
        : callGraph(cGraph), vertices(funs), total_vertices(vertices.size()), fromFunNameToASTFuns(fmap){}



    /*! \brief Return the unique pointer of the call graph for a given program.
     * \param The AST of the program and symbol table
     */

    static std::unique_ptr<CallGraph> build(ASTProgram*, SymbolTable* st);

    /*! \brief Return the total num of vertices for a given call graph.
    */
    int getTotalVertices();

    /*! \brief Return the total num of edges for a given call graph.
    */
    int getTotalEdges();


    /*! \brief Return the set of vertices for a given call graph.
    */
    std::vector<ASTFunction*> getVertices();

    /*! \brief Return the set of edges for a given call graph.
    */
    std::vector<std::pair<ASTFunction*, ASTFunction*>> getEdges();

    /*! \brief Returns all the subroutines called by function f. this is an overloaded function
     * \param f The AST Function node, caller is the string name of a function
     * \return The set of all callee functions node
     */
    std::set<ASTFunction*> getCallees(ASTFunction* f);
    std::set<ASTFunction*> getCallees(std::string caller);


    /*! \brief Returns all the subroutines that call function f.
     * \param f The AST Function node or sting name of the function
     * \return The set of all callers functions node
     */
    std::set<ASTFunction*> getCallers(ASTFunction* f);
    std::set<std::string> getCallers(std::string callee);

    //! Print call graph contents to output stream
    void print(std::ostream& os);

    /*! \brief Returns bool value indicating whether there is an edge between two subroutines.
     * \param caller and callee are the str name of the subroutines
     * \return returns true if an edge exists otherwise false
     */
    bool existEdge(std::string caller, std::string callee);


    /*! \brief Returns the ASTFunction* for a given function name .
     * \param str name of the subroutines
     * \return ASTFunction*
     */
    ASTFunction* getASTFun(std::string f_name);

};
