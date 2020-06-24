/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Семантический парсер для создания абстрактного синтаксического дерева на основе context-free grammar
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYNTAX_ANALYSIS_HPP
#define ORIGAMI_SYNTAX_ANALYSIS_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/lexical_analyzer/lexical_analyzer.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_cpp.hpp"
#include "origami_lexical/conventions/tokens.hpp"

#include <optional>
#include <any>
#include <utility>

namespace origami::parser {

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
  AstNode() = default;

  virtual ~AstNode() = default;

  AstNode(const AstNode&) = default;

  AstNode& operator=(const AstNode&) = default;

  AstNode(AstNode&&) noexcept = default;

  AstNode& operator=(AstNode&&) noexcept = default;

  ///< \brief     Главная функция узла, которая выполняет указанную операцию над данными и возвращает результат этой операции
  [[nodiscard]] virtual std::any accept(AstVisitor& t_visitor) = 0;

  ///< \brief     Задание левого узла дерева
  void addLeft(const std::shared_ptr<AstNode>& t_child);

  ///< \brief     Задание правого узла дерева
  void addRight(const std::shared_ptr<AstNode>& t_child);

  ///< \brief     Получить доступ к левому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstNode> left() const;

  ///< \brief     Получить доступ к правому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstNode> right() const;

private:
  std::shared_ptr<AstNode> m_left; ///< Левый дочерний узел
  std::shared_ptr<AstNode> m_right; ///< Правый дочерний узел
};

/// Поддерживаемые операции Абстрактного синтаксического дерева
class AstNodeNumber; ///< Хранение данных
class AstNodeAdder; ///< Операция суммирования данных

class AstVisitor
{
public:
  AstVisitor() = default;

  virtual ~AstVisitor() = default;

  AstVisitor(const AstVisitor&) = default;

  AstVisitor& operator=(const AstVisitor&) = default;

  AstVisitor(AstVisitor&&) noexcept = default;

  AstVisitor& operator=(AstVisitor&&) noexcept = default;

  [[nodiscard]] virtual std::any visitSumNode(AstNodeAdder& /*t_node*/);

  [[nodiscard]] virtual std::any visitValueNode(AstNodeNumber& /*t_node*/);
};

///< Узел дерева для хранения данных
struct AstNodeNumber : public AstNode
{
public:
  friend class AstVisitor;

  explicit AstNodeNumber(std::any t_data) : m_value(std::move(t_data)) {}

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visitValueNode(*this); }

private:
  std::any doing() const { return m_value; }

  std::any m_value;
};

///< Узел дерева для суммирования данных
struct AstNodeAdder : public AstNode
{
public:
  friend class AstVisitor;

  std::any accept(AstVisitor& t_visitor) override { return t_visitor.visitSumNode(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

class SyntaxAnalyzerCpp
{
private:
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP