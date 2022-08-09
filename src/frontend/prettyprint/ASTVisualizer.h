#pragma once

#include "ASTVisitor.h"
#include <ostream>
#include <iostream>
#include <string>
#include <stack>
#include <map>
#include <SyntaxTree.h>

/*! \brief Constructs a dot graph that can be used to visualize the AST.
 *
 * The visualizer traverses the AST in a pre-order fashion, connecting, with edges, children to
 * parents. The output adheres to the dot syntax and can be converted to a format suitable for visualization.
 *
 * For example, if you run the visualizer and write its output to a file called foo.gv, you can build
 * a png from it using the dot command line interface.
 * \code{.sh}
 * dot -Tpng -o foo.png foo.gv
 * \endcode
 *
 */
class ASTVisualizer {
public:
  ASTVisualizer() : os(std::cout) {}
  ASTVisualizer(std::ostream &os) : os(os) {}

  /*! \brief Construct a dot syntax compliant graph of the AST.
   *
   * \param tree The root of the AST.
   */
  void buildGraph(SyntaxTree &tree);

private:
  std::ostream &os;
  std::stack<ASTNode *> parent;
  std::map<ASTNode *,std::string> vertexMap;

  void connect_node_to_parent(ASTNode * element);
  void declare_node(ASTNode * element, std::string label = "");
  void pushn(ASTNode * element, int n);
  void process_node(ASTNode * element);
};

