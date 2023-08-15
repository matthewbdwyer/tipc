#pragma once

#include "TipCons.h"
#include "TipVar.h"
#include <string>

/*! \class TipAlpha
 *  \brief Class for free type variables
 */
class TipAlpha : public TipVar {
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

  /*! \brief Constructor for creating context specific alphas
   *
   * \param node The ASTNode for the original alpha
   * \param node The ASTNode for its usage context
   * \param name The field name or empty
   */
  TipAlpha(ASTNode *node, ASTNode *context, std::string const name);

  ASTNode *getContext() const;
  std::string const &getName() const;

  bool operator==(const TipType &other) const override;
  bool operator!=(const TipType &other) const override;

  void accept(TipTypeVisitor *visitor) override;

protected:
  // Node for distinguishing free type variables based on usage context
  ASTNode *context;

  std::ostream &print(std::ostream &out) const override;

  std::string const name;
};
