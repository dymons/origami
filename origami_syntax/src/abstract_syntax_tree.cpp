/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#include "origami_syntax/parsing/abstract_syntax_tree.hpp"

namespace origami::ast {
AstBase::AstBase() : m_left_child{ nullptr }, m_right_child{ nullptr }
{}

AstBase::AstBase(const std::shared_ptr<AstBase>& t_left_child, const std::shared_ptr<AstBase>& t_right_child)
  : m_left_child{ t_left_child }, m_right_child{ t_right_child }
{}

void AstBase::setLeftChild(const std::shared_ptr<AstBase>& t_child)
{
  m_left_child = t_child;
}

void AstBase::setRightChild(const std::shared_ptr<AstBase>& t_child)
{
  m_right_child = t_child;
}

std::shared_ptr<AstBase> AstBase::getLeftChild() const noexcept
{
  return m_left_child;
}

std::shared_ptr<AstBase> AstBase::getRightChild() const noexcept
{
  return m_right_child;
}

AstNumber::AstNumber(std::any t_value) : m_value(std::move(t_value))
{}

AstNumber::AstNumber(std::any t_value, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right)
  : m_value(std::move(t_value)), AstNode{ t_left, t_right }
{}

void AstNumber::setValue(std::any t_value)
{
  m_value = std::move(t_value);
}

std::any AstNumber::getValue() const noexcept
{
  return m_value;
}

AstMathOperator::AstMathOperator(std::string t_operator) : m_operator(std::move(t_operator))
{}

AstMathOperator::AstMathOperator(std::string t_operator, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right)
  : m_operator(std::move(t_operator)), AstNode{ t_left, t_right }
{}
void AstMathOperator::setOperator(std::string t_operator)
{
  m_operator = std::move(t_operator);
}

std::string AstMathOperator::getOperator() const noexcept
{
  return m_operator;
}

template<concepts::Arithmetic... Ts>
auto AstMathOperator::execute(Ts&&... t_data) const -> typename std::common_type_t<Ts...>
{
  // clang-format off
  switch (const auto hash = utility::fnv1a::hash(m_operator); hash) {
    case utility::fnv1a::hash("+"): {
      return (t_data + ... + 0);
    }
    case utility::fnv1a::hash("-"): {
      return (t_data - ... - 0);
    }
    case utility::fnv1a::hash("/"): {
      return (t_data / ... / 1);
    }
    case utility::fnv1a::hash("*"): {
      return (t_data * ... * 1);
    }
    [[unlikely]] default: {
      throw UnsupportedOperationError{ fmt::format("Неподдерживаемая операция {0} ", m_operator) };
    }
  }
  // clang-format on
}

std::any AstVisitor::visit(const AstNumber& t_node)
{
  return t_node.getValue();
}

std::any AstVisitor::visit(const AstMathOperator& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) {
    return {};
  }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) {
    return {};
  }

  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.execute(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.execute(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.execute(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.execute(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format(
      "Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
  }
}
}// namespace origami::ast