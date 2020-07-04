/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#include "origami_syntax/parsing/abstract_syntax_tree.hpp"
#include "origami_lexical/conventions/exeptions.hpp"

#include <fmt/core.h>

namespace origami::ast {
AstNode::AstNode(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right)
{
  m_left = t_left;
  m_right = t_right;
}

void AstNode::setLeftChild(const std::shared_ptr<AstNode>& t_child) { m_left = t_child; }
void AstNode::setRightChild(const std::shared_ptr<AstNode>& t_child) { m_right = t_child; };
std::shared_ptr<AstNode> AstNode::getLeftChild() const { return m_left; }
std::shared_ptr<AstNode> AstNode::getRightChild() const { return m_right; }

AstNodeNumber::AstNodeNumber(std::any t_data) : m_value(std::move(t_data)) {}
AstNodeNumber::AstNodeNumber(std::any t_data, const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right)
: m_value(std::move(t_data)), AstNode(t_left, t_right)
{}
std::any AstNodeNumber::accept(AstVisitor& t_visitor) { return t_visitor.visit(*this); }
std::any AstNodeNumber::doing() const noexcept { return m_value; }

std::any AstVisitor::visit(AstNodeNumber& t_node) { return t_node.doing(); }
std::any AstVisitor::visit(AstNodeAdder& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  // TODO: Найти решение по работе с данными, посмотреть в сторону паттернов.
  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format(
      "Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
  }
}

std::any AstVisitor::visit(AstNodeSubtractor& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format(
      "Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
  }
}

std::any AstVisitor::visit(AstNodeMultiplier& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format(
      "Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
  }
}

std::any AstVisitor::visit(AstNodeDivider& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format(
      "Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
  }
}
}// namespace origami::ast