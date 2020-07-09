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