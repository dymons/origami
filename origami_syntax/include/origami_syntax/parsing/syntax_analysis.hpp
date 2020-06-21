/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYNTAX_ANALYSIS_HPP
#define ORIGAMI_SYNTAX_ANALYSIS_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/lexical_analyzer/lexical_analyzer.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_cpp.hpp"
#include "origami_lexical/conventions/tokens.hpp"

#include <optional>

namespace origami::parser {

///< Идея абстрактного синтаксическго дерева позаимствована из https://github.com/wangshan/ast-visitor
class AstNodeVisitor;

template<typename T> class Data
{
public:
  using Type = T;
};

template<typename T> class AstVisitable
{
public:
  [[nodiscard]] virtual std::optional<T> accept(Data<T> /*unused*/, AstNodeVisitor& /*unused*/)
  {
#ifdef ORIGAMI_DEBUG
    assert(false && ": переопределите функцию");
#endif
    return {};
  };

  AstVisitable() = default;

  virtual ~AstVisitable() = default;

  AstVisitable(const AstVisitable&) = default;

  AstVisitable& operator=(const AstVisitable&) = default;

  AstVisitable(AstVisitable&&) noexcept = default;

  AstVisitable& operator=(AstVisitable&&) noexcept = default;
};

class AstNode : virtual AstVisitable<int>, AstVisitable<double>
{
public:
  using AstVisitable<int>::accept;
  using AstVisitable<double>::accept;

  void addLeft(const std::shared_ptr<AstNode>& t_child);
  void addRight(const std::shared_ptr<AstNode>& t_child);

  [[nodiscard]] std::shared_ptr<AstNode> left() const;
  [[nodiscard]] std::shared_ptr<AstNode> right() const;

private:
  std::shared_ptr<AstNode> m_left;
  std::shared_ptr<AstNode> m_right;
};

template<typename T> class ValueNode;
template<typename T, typename U> class SumNode;

class AstNodeVisitor
{
public:
  template<typename T, typename U>[[nodiscard]] std::optional<std::common_type_t<T, U>> visitSumNode(SumNode<T, U>& /*t_node*/);

  template<typename T>[[nodiscard]] std::optional<T> visitValueNode(ValueNode<T>& /*t_node*/);
};

///< Хранение чисел
template<typename T> class ValueNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  explicit ValueNode(T t_data) : m_value(t_data) {}

  std::optional<T> accept(Data<T> /*unused*/, AstNodeVisitor& t_visitor) override { return t_visitor.visitValueNode<T>(*this); }

private:
  T doing() const { return m_value; }

  T m_value;
};

//template class ValueNode<int>;
//template class ValueNode<double>;

///< Суммирование чисел
template<typename T, typename U> class SumNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  std::optional<std::common_type_t<T, U>> accept(Data<std::common_type_t<T, U>> /*unused*/, AstNodeVisitor& t_visitor) override
  {
    return t_visitor.visitSumNode<T, U>(*this);
  }

private:
  auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

//template class SumNode<int, int>;
//template class SumNode<int, double>;

template<typename T, typename U> std::optional<std::common_type_t<T, U>> AstNodeVisitor::visitSumNode(SumNode<T, U>& t_node)
{
  const std::optional<T> lhs = t_node.left()->accept(Data<T>(), *this);
  const std::optional<U> rhs = t_node.right()->accept(Data<U>(), *this);
  if (!lhs || !rhs) { return {}; }

  return t_node.doing(*lhs, *rhs);
}

template<typename T> std::optional<T> AstNodeVisitor::visitValueNode(ValueNode<T>& t_node) { return t_node.doing(); }

class SyntaxAnalysis
{
private:
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP