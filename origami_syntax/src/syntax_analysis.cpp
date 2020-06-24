#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"

#include <fmt/core.h>

namespace origami::parser {
void AstNode::addLeft(const std::shared_ptr<AstNode>& t_child) { m_left = t_child; }
void AstNode::addRight(const std::shared_ptr<AstNode>& t_child) { m_right = t_child; };
std::shared_ptr<AstNode> AstNode::left() const { return m_left; }
std::shared_ptr<AstNode> AstNode::right() const { return m_right; }

std::any AstNodeVisitor::visitValueNode(ValueNode& t_node) { return t_node.doing(); }

std::any AstNodeVisitor::visitSumNode(SumNode& t_node)
{
  const std::any lhs = t_node.left()->accept(*this);
  const std::any rhs = t_node.right()->accept(*this);

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