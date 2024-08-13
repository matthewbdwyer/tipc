#include "ASTVisitor.h"

template<class N>
class NodeFinder : public ASTVisitor {
public:
    static std::shared_ptr<N> find_node(ASTProgram *p) {
        NodeFinder<N> visitor;
        p->accept(&visitor);
        return visitor.found_node;
    }

    virtual void endVisit(N *element) {  found_node = std::make_shared<N>(*element); };

private:
    std::shared_ptr<N> found_node;
};
