#include "ASTVisualizer.h"
#include "Iterator.h"

#include <iostream>
#include <sstream>

void ASTVisualizer::buildGraph(SyntaxTree &tree) {
  os << "digraph {" << "\n";
  os << "  rankdir = TB;" << "\n";
  os << "\n";

  for (auto iter = tree.begin(""); iter != tree.end(""); ++iter) {
    auto node = iter->getRoot().get();
    if(parent.empty()) {
      std::string vertexName = "v"+std::to_string(vertexMap.size());  
      vertexMap.insert(std::pair<ASTNode*, std::string>(node,vertexName));
      declare_node(node, "start");
      pushn(node, node->getChildren().size());
    } else {
      process_node(node);
    }
  }

  os << "}";
}

void ASTVisualizer::process_node(ASTNode * element) {
  /*
   * We construct names for vertices in the dot representation to make 
   * the visualizer output consistent across runs on the same program.
   * The assumption is that nodes are processed in pre-order so that
   * the name of the parent is generated before a child is processed.
   */
  std::string vertexName = "v"+std::to_string(vertexMap.size());  
  vertexMap.insert(std::pair<ASTNode*, std::string>(element,vertexName));

  declare_node(element);
  connect_node_to_parent(element);
  pushn(element, element->getChildren().size());
}

void ASTVisualizer::declare_node(ASTNode * element, std::string label) {
  std::stringstream l(label) ;
  if(label.empty()) {
    l << *element;
  }
  os << "  " << '"' << vertexMap[element] << '"' << " [label = " << '"' << l.str() << '"' << "];" << "\n";
}

void ASTVisualizer::connect_node_to_parent(ASTNode * element) {
  if(parent.empty()) {
    return;
  }

  os << "  " << '"' << vertexMap[parent.top()] << '"' << " -> " << '"' << vertexMap[element] << '"' << ";\n";
  parent.pop();
}

void ASTVisualizer::pushn(ASTNode * element, int n) {
  for(int i = 0; i < n; i++) {
    parent.push(element);
  }
}
