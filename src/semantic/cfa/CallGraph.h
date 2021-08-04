#pragma once

#include <map>
#include <vector>
#include "ASTVisitor.h"
#include "treetypes/AST.h"
#include "SymbolTable.h"
#include "CGB.h"
#include <set>

/*! \class CallGraph
 *  \brief Performs control flow analysis and records results for subsequent phases
 *  Generates call graph of a program that represents calling relationships between subroutines
 *  in a computer program. Each node represents a procedure and each edge (a1, a0) indicates that procedure a1 calls procedure a0.
 *  A cycle in the graph indicates recursive procedure calls, e.g., a0 -> a0 indicates that a0 calls itself recursively
 *  this call graph is sometimes approximations. Not all the call relationship that exist in the graph will occur in the actual runs of the program.
 */

class CallGraph {

    std::vector<ASTFunction*> vertices;
    std::vector<std::pair<ASTFunction*, ASTFunction*> > edges;
    int total_edges;
    int total_vertices;
    std::map<ASTFunction*, std::set<ASTFunction*> > callGraph;

public:
    static std::unique_ptr<CallGraph> build(ASTProgram*, SymbolTable* st);

    CallGraph(std::map<ASTFunction*, std::set<ASTFunction*> > cGraph, std::vector<ASTFunction*> funs)
        : callGraph(cGraph), vertices(funs), total_vertices(vertices.size()){}

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
    std::vector<std::pair<ASTFunction*, ASTFunction*> > getEdges();

    /*! \brief Returns all the subroutines called by function f.
     * \param f The AST Function node
     * \return The set of all callee functions node
     */
    std::set<ASTFunction*> getCallees(ASTFunction* f);

    /*! \brief Returns all the subroutines that call function f.
     * \param f The AST Function node
     * \return The set of all callers functions node
     */
    std::set<ASTFunction*> getCallers(ASTFunction* f);

    //! Print call graph contents to output stream
    void print(std::ostream& os);

    bool existEdge(std::string caller, std::string callee);

};
