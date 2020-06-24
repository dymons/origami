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

class AstNodeVisitor;

struct AstNode
{
public:
  AstNode() = default;

  virtual ~AstNode() = default;

  AstNode(const AstNode&) = default;

  AstNode& operator=(const AstNode&) = default;

  AstNode(AstNode&&) noexcept = default;

  AstNode& operator=(AstNode&&) noexcept = default;

  [[nodiscard]] virtual std::any accept(AstNodeVisitor& /*unused*/) = 0;

  void addLeft(const std::shared_ptr<AstNode>& t_child);
  void addRight(const std::shared_ptr<AstNode>& t_child);

  [[nodiscard]] std::shared_ptr<AstNode> left() const;
  [[nodiscard]] std::shared_ptr<AstNode> right() const;

private:
  std::shared_ptr<AstNode> m_left;
  std::shared_ptr<AstNode> m_right;
};

class ValueNode;
class SumNode;

class AstNodeVisitor
{
public:
  [[nodiscard]] std::any visitSumNode(SumNode& /*t_node*/);

  [[nodiscard]] std::any visitValueNode(ValueNode& /*t_node*/);
};

///< Хранение чисел
struct ValueNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  explicit ValueNode(std::any t_data) : m_value(std::move(t_data)) {}

  std::any accept(AstNodeVisitor& t_visitor) override { return t_visitor.visitValueNode(*this); }

private:
  std::any doing() const { return m_value; }

  std::any m_value;
};

///< Суммирование чисел
struct SumNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  std::any accept(AstNodeVisitor& t_visitor) override { return t_visitor.visitSumNode(*this); }

private:
  template<typename T, typename U> auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

class SyntaxAnalysis
{
private:
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP