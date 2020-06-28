/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP
#define ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP

#include <any>
#include <memory>

namespace origami::ast {
/**
 * \brief     Абстрактное синтаксическое дерево основано на паттерне Visitor
 *
 * \details   Базовый класс, который выступает для обхода абстрактного синтаксического дерева.
 */
class AstVisitor;

///< Описание узла абстрактного синтакисечского дерева, выступает аналогом бинарного дерева.
struct AstNode
{
public:
  explicit AstNode(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right);

  AstNode() = default;

  virtual ~AstNode() = default;

  AstNode(const AstNode&) = default;

  AstNode& operator=(const AstNode&) = default;

  AstNode(AstNode&&) noexcept = default;

  AstNode& operator=(AstNode&&) noexcept = default;

  ///< \brief     Главная функция узла, которая выполняет указанную операцию над данными и возвращает результат этой операции
  [[nodiscard]] virtual std::any accept(AstVisitor& t_visitor) = 0;

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

/// Поддерживаемые операции Абстрактного синтаксического дерева
class AstNodeNumber;///< Хранение данных
class AstNodeAdder;///< Операция суммирования данных
class AstNodeSubtractor;///< Операция вычитания данных
class AstNodeMultiplier;///< Операция умножения данных
class AstNodeDivider;///< Операция деления данных

class AstVisitor
{
public:
  AstVisitor() = default;

  virtual ~AstVisitor() = default;

  AstVisitor(const AstVisitor&) = default;

  AstVisitor& operator=(const AstVisitor&) = default;

  AstVisitor(AstVisitor&&) noexcept = default;

  AstVisitor& operator=(AstVisitor&&) noexcept = default;

  [[nodiscard]] virtual std::any visit(AstNodeAdder& /*t_node*/);

  [[nodiscard]] virtual std::any visit(AstNodeNumber& /*t_node*/);

  [[nodiscard]] virtual std::any visit(AstNodeSubtractor& /*t_node*/);

  [[nodiscard]] virtual std::any visit(AstNodeMultiplier& /*t_node*/);

  [[nodiscard]] virtual std::any visit(AstNodeDivider& /*t_node*/);
};

///< Узел дерева для хранения данных
struct AstNodeNumber : public AstNode
{
public:
  friend class AstVisitor;

  explicit AstNodeNumber(std::any t_data) : m_value(std::move(t_data)) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visit(*this); }

private:
  std::any doing() const { return m_value; }

  std::any m_value;
};

///< Узел дерева для суммирования данных
struct AstNodeAdder : public AstNode
{
public:
  friend class AstVisitor;

  AstNodeAdder() = default;

  explicit AstNodeAdder(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right) : AstNode(t_left, t_right) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visit(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

struct AstNodeSubtractor : public AstNode
{
public:
  friend class AstVisitor;

  AstNodeSubtractor() = default;

  explicit AstNodeSubtractor(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right) : AstNode(t_left, t_right) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visit(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs - t_rhs; }
};

struct AstNodeMultiplier : public AstNode
{
public:
  friend class AstVisitor;

  AstNodeMultiplier() = default;

  explicit AstNodeMultiplier(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right) : AstNode(t_left, t_right) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visit(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs * t_rhs; }
};

struct AstNodeDivider : public AstNode
{
public:
  friend class AstVisitor;

  AstNodeDivider() = default;

  explicit AstNodeDivider(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right) : AstNode(t_left, t_right) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visit(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs / t_rhs; }
};
}// namespace origami::ast

#endif// ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP