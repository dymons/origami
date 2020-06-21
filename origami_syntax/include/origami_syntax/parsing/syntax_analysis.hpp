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

template<typename... Ts> class Types
{
public:
  using Type = Types;
};

using SupportTypesCpp = Types<int, bool>;

template<typename T> class AstVisitable
{
public:
  [[nodiscard]] virtual std::optional<T> accept(Data<T> /*unused*/, AstNodeVisitor& /*unused*/)
  {
#ifdef ORIGAMI_DEBUG
    assert(false && __func__ && ": переопределите функцию");
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

template<typename T> struct AstVisitables;

template<typename T, typename... Ts>
struct AstVisitables<Types<T, Ts...>>
  : virtual AstVisitable<T>
  , AstVisitables<Types<Ts...>>
{
  using AstVisitable<T>::accept;
};

template<> struct AstVisitables<Types<>>
{
  AstVisitables() = default;

  virtual ~AstVisitables() = default;

  AstVisitables(const AstVisitables&) = default;

  AstVisitables& operator=(const AstVisitables&) = default;

  AstVisitables(AstVisitables&&) noexcept = default;

  AstVisitables& operator=(AstVisitables&&) noexcept = default;
};

class AstNode : public AstVisitables<SupportTypesCpp>
{
public:
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
class CompareNode;

class AstNodeVisitor
{
public:
  template<typename T>[[nodiscard]] std::optional<T> visitSumNode(SumNode& /*t_node*/);

  [[nodiscard]] std::optional<bool> visitCompareNode(CompareNode& /*t_node*/);

  template<typename T>[[nodiscard]] std::optional<T> visitValueNode(ValueNode<T>& /*t_node*/);
};

class SumNode : public AstNode
{
public:
  std::optional<int> accept(Data<int> /*unused*/, AstNodeVisitor& t_visitor) override { return t_visitor.visitSumNode<int>(*this); }

  template<typename T, typename U>
  auto doing(const T t_lhs, const U t_rhs) -> typename std::common_type_t<T, U> { return t_lhs + t_rhs; }
};

class CompareNode : public AstNode
{
public:
  std::optional<bool> accept(Data<bool> /*unused*/, AstNodeVisitor& t_visitor) override { return t_visitor.visitCompareNode(*this); }

  template<typename T, typename U>
  bool doing(T t_lhs, U t_rhs) const { return t_lhs < t_rhs; }
};

template<typename T> class ValueNode : public AstNode
{
public:
  explicit ValueNode(T t_data) : m_value(t_data) {}

  std::optional<T> accept(Data<T> /*unused*/, AstNodeVisitor& t_visitor) override { return t_visitor.visitValueNode<T>(*this); }

  T doing() const { return m_value; }

private:
  T m_value;
};

template class ValueNode<int>;

template<typename T> std::optional<T> AstNodeVisitor::visitSumNode(SumNode& t_node)
{
  const auto lhs = t_node.left()->accept(Data<T>(), *this);
  const auto rhs = t_node.right()->accept(Data<T>(), *this);
  if (!lhs || !rhs) { return {}; }

  return t_node.doing(*lhs, *rhs);
}

inline std::optional<bool> AstNodeVisitor::visitCompareNode(CompareNode& t_node)
{
  const auto lhs = t_node.left()->accept(Data<int>(), *this);
  const auto rhs = t_node.right()->accept(Data<int>(), *this);
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