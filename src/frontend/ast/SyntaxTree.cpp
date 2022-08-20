#include "SyntaxTree.h"
#include "Iterator.h"
#include "PreOrderIterator.h"

SyntaxTree::SyntaxTree(std::shared_ptr<ASTNode> node) : root(node) { }

SyntaxTree::~SyntaxTree() = default;

SyntaxTree::SyntaxTree(const SyntaxTree &tree) = default;

Iterator SyntaxTree::begin(const std::string &order) {
  // Don't fret, the Iterator class is going to manage this pointer for us.
  PreOrderIterator * testIterator = new PreOrderIterator(*this, false);
  return Iterator(testIterator);
}

Iterator SyntaxTree::end(const std::string &order) {
  PreOrderIterator * testIterator = new PreOrderIterator(*this, true);
  return Iterator(testIterator);
}

std::vector<SyntaxTree> SyntaxTree::getSubtrees() {
  std::vector<SyntaxTree> subtrees;
  for(auto child : root->getChildren()) {
    subtrees.push_back(SyntaxTree(child));
  }

  return subtrees;
}

SyntaxTree &SyntaxTree::operator=(const SyntaxTree &tree) {
  if(this != &tree) {
    root = tree.root;
  }
  return *this;
}

void SyntaxTree::accept(ASTVisitor *visitor) {
  root->accept(visitor);
}

std::shared_ptr<ASTNode> SyntaxTree::getRoot() const {
  return root;
}

