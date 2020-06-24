#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"

#include <fmt/core.h>

namespace origami::parser {
void AstNode::setLeftChild(const std::shared_ptr<AstNode>& t_child) { m_left = t_child; }
void AstNode::setRightChild(const std::shared_ptr<AstNode>& t_child) { m_right = t_child; };
std::shared_ptr<AstNode> AstNode::getLeftChild() const { return m_left; }
std::shared_ptr<AstNode> AstNode::getRightChild() const { return m_right; }

std::any AstVisitor::visit(AstNodeNumber& t_node) { return t_node.doing(); }
std::any AstVisitor::visit(AstNodeAdder& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) { return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs)); }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  }

  if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  }

  throw UnsupportedOperationError{ fmt::format("Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
}
}// namespace origami::parser