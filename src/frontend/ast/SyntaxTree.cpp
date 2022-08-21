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
} // LCOV_EXCL_LINE

Iterator SyntaxTree::end(const std::string &order) {
  PreOrderIterator * testIterator = new PreOrderIterator(*this, true);
  return Iterator(testIterator);
} // LCOV_EXCL_LINE

std::vector<SyntaxTree> SyntaxTree::getSubtrees() {
  std::vector<SyntaxTree> subtrees;
  for(auto child : root->getChildren()) {
    subtrees.push_back(SyntaxTree(child));
  }

  return subtrees;
} // LCOV_EXCL_LINE

SyntaxTree &SyntaxTree::operator=(const SyntaxTree &tree) {
  if(this != &tree) {
    root = tree.root;
  }
  return *this;
} // LCOV_EXCL_LINE

void SyntaxTree::accept(ASTVisitor *visitor) {
  root->accept(visitor);
} // LCOV_EXCL_LINE

std::shared_ptr<ASTNode> SyntaxTree::getRoot() const {
  return root;
} // LCOV_EXCL_LINE

