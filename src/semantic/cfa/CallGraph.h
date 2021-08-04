#pragma once

#include <map>
#include <vector>
#include "ASTVisitor.h"
#include "treetypes/AST.h"

#include <set>

/*! \class CallGraph
 *  \brief Performs control flow analysis and records results for subsequent phases.
 */
class CallGraph {

    std::vector<ASTFunction*> vertices;
    std::vector<std::pair<ASTFunction*, ASTFunction*>> edges;
    int total_edges;
    int total_vertices;
    std::map<ASTFunction*, std::set<ASTFunction*> > callGraph;

public:
    CallGraph(std::map<ASTFunction*, std::set<ASTFunction*> > cGraph, std::vector<ASTFunction*> funs )
        : callGraph(cGraph), vertices(funs), total_vertices(cGraph.size()) {}

    /*! \brief Return the total num of vertices for a given call graph.
    */
    int getTotalVertices();
    /*! \brief Return the total num of edges for a given call graph.
    */
    int getTotalEdges();
    void setTotalEdges();

    /*! \brief Return the set of vertices for a given call graph.
    */
    std::vector<ASTFunction*> getVertices();

    /*! \brief Return the set of edges for a given call graph.
    */
    std::vector<std::pair<ASTFunction*, ASTFunction*>> getEdges();

    /*! \brief Returns all the subroutines called by function f.
     * \param s The AST Function node
     * \return The set of all callee functions node
     */
   // std::set<ASTFunction*> getCallees(ASTFunction* f);

    /*! \brief Returns all the subroutines that call function f.
     * \param s The AST Function node
     * \return The set of all callers functions node
     */
    //std::set<ASTFunction*> getCallers(ASTFunction* f);

    //! Print call graph contents to output stream
    void print(std::ostream &os);
};
