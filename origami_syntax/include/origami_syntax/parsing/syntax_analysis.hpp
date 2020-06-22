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

template<typename T> class SingleData
{
public:
  using Type = T;
};

template<typename T> struct AstVisitable
{
public:
  [[nodiscard]] virtual std::optional<T> accept(SingleData<T> /*unused*/, AstNodeVisitor& /*unused*/)
  {
#ifdef ORIGAMI_DEBUG
    assert(false && ": функция не определена");
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

struct AstNode : virtual AstVisitable<int>, AstVisitable<double>
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
class SumNode;

class AstNodeVisitor
{
public:
  template<typename T, typename U>[[nodiscard]] std::optional<std::common_type_t<T, U>> visitSumNode(SumNode& /*t_node*/);

  template<typename T>[[nodiscard]] std::optional<T> visitValueNode(ValueNode<T>& /*t_node*/);
};

///< Хранение чисел
template<typename T> struct ValueNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  explicit ValueNode(T t_data) : m_value(t_data) {}

  std::optional<T> accept(SingleData<T> /*unused*/, AstNodeVisitor& t_visitor) override { return t_visitor.visitValueNode<T>(*this); }

private:
  T doing() const { return m_value; }

  T m_value;
};

///< Суммирование чисел
struct SumNode : public AstNode
{
public:
  friend class AstNodeVisitor;

  std::optional<int> accept(SingleData<int> /*unused*/, AstNodeVisitor& t_visitor) override
  {
    return t_visitor.visitSumNode<int, int>(*this);
  }

  std::optional<double> accept(SingleData<double> /*unused*/, AstNodeVisitor& t_visitor) override
  {
    return t_visitor.visitSumNode<int, double>(*this); ///< int, для теста, нужно принимать два типа данных
  }

private:
  template<typename T, typename U>
  auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

template<typename T, typename U> std::optional<std::common_type_t<T, U>> AstNodeVisitor::visitSumNode(SumNode& t_node)
{
  const std::optional<T> lhs = t_node.left()->accept(SingleData<T>(), *this);
  const std::optional<U> rhs = t_node.right()->accept(SingleData<U>(), *this);
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