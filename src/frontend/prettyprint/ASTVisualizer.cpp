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
      declare_node(node, "start");
      pushn(node, node->getChildren().size());
    } else {
      process_node(node);
    }
  }

  os << "}";
}

void ASTVisualizer::process_node(ASTNode * element) {
  declare_node(element);
  connect_node_to_parent(element);
  pushn(element, element->getChildren().size());
}

void ASTVisualizer::declare_node(ASTNode * element, std::string label) {
  std::stringstream l(label) ;
  if(label.empty()) {
    l << *element;
  }
  os << "  " << '"' << element << '"' << " [label = " << '"' << l.str() << '"' << "];" << "\n";
}

void ASTVisualizer::connect_node_to_parent(ASTNode * element) {
  if(parent.empty()) {
    return;
  }

  os << "  " << '"' << parent.top() << '"' << " -> " << '"' << element << '"' << ";\n";
  parent.pop();
}

void ASTVisualizer::pushn(ASTNode * element, int n) {
  for(int i = 0; i < n; i++) {
    parent.push(element);
  }
}
