#pragma once

#include "TipVar.h"
#include "TipCons.h"
#include <string>

/*! \class TipAlpha
 *  \brief Class for free type variables
 */
class TipAlpha: public TipVar {
public:
    TipAlpha() = delete;

    /*! \brief Constructor for all non-record nodes
     *
     * \param node The expression or decl node
     */
    TipAlpha(ASTNode *node); 

    /*! \brief Constructor for record or access exprs
     *
     * \param node The record or access expr
     * \param name The field name
     */
    TipAlpha(ASTNode *node, std::string const name); 

    std::string const & getName() const;

    bool operator==(const TipType& other) const override;
    bool operator!=(const TipType& other) const override;

    void accept(TipTypeVisitor *visitor) override;

protected:
    std::ostream& print(std::ostream &out) const override;

    std::string const name;
};


