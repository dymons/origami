/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP
#define ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP

#include "origami_lexical/conventions/exeptions.hpp"

#include <any>
#include <memory>

#include <fmt/core.h>

namespace origami::ast::crtp {

namespace fnv1a {
  template<typename Itr> static constexpr std::uint32_t hash(Itr begin, Itr end) noexcept
  {
    std::uint32_t h = 0x811c9dc5;

    while (begin != end) {
      h = (h ^ (*begin)) * 0x01000193;
      ++begin;
    }
    return h;
  }

  template<size_t N> static constexpr std::uint32_t hash(const char (&str)[N]) noexcept { return hash(std::begin(str), std::end(str) - 1); }

  static constexpr std::uint32_t hash(const std::string_view& sv) noexcept { return hash(sv.begin(), sv.end()); }

  static inline std::uint32_t hash(const std::string& s) noexcept { return hash(s.begin(), s.end()); }
}// namespace fnv1a

class AstBase;
class AstNumber;
class AstNodeMathOperator;

class AstVisitor
{
public:
  std::any visit(const AstNumber& t_node);
  std::any visit(const AstNodeMathOperator& t_node);
};

class AstBase
{
public:
  AstBase() : m_left{ nullptr }, m_right{ nullptr } {}

  AstBase(const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right) : m_left{ t_left }, m_right{ t_right } {}

  virtual std::any accept(AstVisitor& t_visitor) const = 0;

  virtual ~AstBase() = default;

  ///< \brief     Задание левого узла дерева
  void setLeftChild(const std::shared_ptr<AstBase>& t_child) { m_left = t_child; }

  ///< \brief     Задание правого узла дерева
  void setRightChild(const std::shared_ptr<AstBase>& t_child) { m_right = t_child; }

  ///< \brief     Получить доступ к левому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstBase> getLeftChild() const { return m_left; }

  ///< \brief     Получить доступ к правому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstBase> getRightChild() const { return m_right; }

private:
  std::shared_ptr<AstBase> m_left;///< Левый дочерний узел
  std::shared_ptr<AstBase> m_right;///< Правый дочерний узел
};

template<typename T> class AstNode : public AstBase
{
public:
  AstNode() = default;

  AstNode(const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right) : AstBase{ t_left, t_right } {}

  std::any accept(AstVisitor& t_visitor) const final
  {
    if (const auto* upcast = dynamic_cast<const T* const>(this); upcast) { return t_visitor.visit(*upcast); }

    return {};
  }
};

class AstNumber : public AstNode<AstNumber>
{
public:
  explicit AstNumber(std::any t_value) : m_value(std::move(t_value)) {}

  AstNumber(std::any t_value, const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right)
    : m_value(std::move(t_value)), AstNode{ t_left, t_right }
  {}

  void setValue(std::any t_value) { m_value = std::move(t_value); }

  std::any getValue() const { return m_value; }

private:
  std::any m_value;
};

template<typename... Ts> concept Arithmetic = std::conjunction_v<std::is_arithmetic<Ts>...>;

class AstNodeMathOperator : public AstNode<AstNodeMathOperator>
{
public:
  explicit AstNodeMathOperator(std::string t_operator) : m_operator(std::move(t_operator)) {}

  explicit AstNodeMathOperator(std::string t_operator, const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right)
    : m_operator(std::move(t_operator)), AstNode{ t_left, t_right }
  {}

  template<Arithmetic... Ts> auto execute(Ts&&... t_data) const -> typename std::common_type_t<Ts...>
  {
    switch (const auto hash = fnv1a::hash(m_operator); hash) {
      case fnv1a::hash("+"): {
        return (t_data + ... + 0);
      }
      case fnv1a::hash("-"): {
        return (t_data - ... - 0);
      }
      case fnv1a::hash("/"): {
        return (t_data / ... / 1);
      }
      case fnv1a::hash("*"): {
        return (t_data * ... * 1);
      }
      default: {
        throw UnsupportedOperationError{ fmt::format("Неподдерживаемая операция {0} ", m_operator) };
      }
    }
  }

private:
  std::string m_operator;
};

inline std::any AstVisitor::visit(const AstNumber& t_node) { return t_node.getValue(); }

inline std::any AstVisitor::visit(const AstNodeMathOperator& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

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

}// namespace origami::ast::crtp

namespace origami::ast {
/**
 * \brief     Абстрактное синтаксическое дерево основано на паттерне Visitor
 *
 * \details   Базовый класс, который выступает для обхода абстрактного синтаксического дерева.
 */
class AstVisitor;

/// Поддерживаемые операции Абстрактного синтаксического дерева
class AstNodeNumber;///< Узел дерева для хранение данных различного типа
class AstNodeMathOperator;///< Узел дерева для работы с математическими операциями

class AstVisitor
{
public:
  AstVisitor() = default;

  virtual ~AstVisitor() = default;

  AstVisitor(const AstVisitor&) = default;

  AstVisitor& operator=(const AstVisitor&) = default;

  AstVisitor(AstVisitor&&) noexcept = default;

  AstVisitor& operator=(AstVisitor&&) noexcept = default;

  [[nodiscard]] virtual std::any visit(AstNodeNumber& /*t_node*/);

  [[nodiscard]] virtual std::any visit(AstNodeMathOperator& /*t_node*/);
};

///< Описание узла абстрактного синтакисечского дерева, выступает аналогом бинарного дерева.
struct AstNode
{
public:
  ///< \brief     Пользовательский конструктор. Инициализация левого и правого узла.
  AstNode(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right);

  ///< \brief     Конструктор по умолчанию
  AstNode() : m_left{ nullptr }, m_right{ nullptr } {}

  ///< \brief     Виртуальный деструктор
  virtual ~AstNode() = default;

  ///< \brief     Конструктор копирования
  AstNode(const AstNode&) = default;

  ///< \brief     Оператор копирования
  AstNode& operator=(const AstNode&) = default;

  ///< \brief     Конструктор перемещения
  AstNode(AstNode&&) noexcept = default;

  ///< \brief     Оператор перемещения
  AstNode& operator=(AstNode&&) noexcept = default;

  ///< \brief     Главная функция узла, которая выполняет указанную операцию над данными и возвращает результат этой операции
  virtual std::any accept(AstVisitor& t_visitor) = 0;

  ///< \brief     Задание левого узла дерева
  void setLeftChild(const std::shared_ptr<AstNode>& t_child);

  ///< \brief     Задание правого узла дерева
  void setRightChild(const std::shared_ptr<AstNode>& t_child);

  ///< \brief     Получить доступ к левому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstNode> getLeftChild() const;

  ///< \brief     Получить доступ к правому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstNode> getRightChild() const;

private:
  std::shared_ptr<AstNode> m_left;///< Левый дочерний узел
  std::shared_ptr<AstNode> m_right;///< Правый дочерний узел
};

class AstNodeNumber : public AstNode
{
public:
  friend class AstVisitor;

  explicit AstNodeNumber(std::any t_data);

  explicit AstNodeNumber(std::any t_data, const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right);

  [[nodiscard]] std::any accept(AstVisitor& t_visitor) override;

private:
  std::any doing() const noexcept;

  std::any m_value;
};

class AstNodeMathOperator : public AstNode
{
public:
  friend class AstVisitor;

  explicit AstNodeMathOperator(std::string t_operator);

  explicit AstNodeMathOperator(std::string t_operator, const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right);

  [[nodiscard]] std::any accept(AstVisitor& t_visitor) override;

private:
  template<typename... Ts> auto doing(Ts&&... t_data) -> typename std::common_type_t<Ts...>
  {
    if (m_operator == "+") { return (t_data + ... + 0); }
    if (m_operator == "-") { return (t_data - ... - 0); }
    if (m_operator == "/") { return (t_data / ... / 1); }
    if (m_operator == "*") { return (t_data * ... * 1); }

    throw UnsupportedOperationError{ fmt::format("Неподдерживаемая операция {0} ", m_operator) };
  }

  std::string m_operator;
};

}// namespace origami::ast

#endif// ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP