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

using SupportTypesCpp = Types<int, bool>;

class AstNode : public AstVisitables<SupportTypesCpp>
{
public:
  void addLeft(AstNode* child) { m_left = child; }
  void addRight(AstNode* child) { m_right = child; }
  [[nodiscard]] AstNode* left() { return m_left; }
  [[nodiscard]] AstNode* right() { return m_right; }

private:
  AstNode* m_left;
  AstNode* m_right;
};

class SumNode;
class CompareNode;

template <typename T>
class ValueNode;

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

  int doing(int t_lhs, int t_rhs) const { return t_lhs + t_rhs; }
};

class CompareNode : public AstNode
{
public:
  std::optional<bool> accept(Data<bool>, AstNodeVisitor& v) override { return v.visitCompareNode(*this); }

  bool doing(int lhs, int rhs) const { return lhs < rhs; }
};

template<typename T>
class ValueNode : public AstNode
{
private:
  T m_value;

public:
  explicit ValueNode(T x) : m_value(x) {}

  std::optional<T> accept(Data<T>, AstNodeVisitor& v) override { return v.visitValueNode<T>(*this); }

  T doing() const { return m_value; }
};

template class ValueNode<int>;

template<typename T> std::optional<T> AstNodeVisitor::visitSumNode(SumNode& t_node)
{
  const auto lhs = t_node.left()->accept(Data<T>(), *this);
  const auto rhs = t_node.right()->accept(Data<T>(), *this);
  if (!lhs || !rhs) { return {}; }

  return t_node.doing(*lhs, *rhs);
}

inline std::optional<bool> AstNodeVisitor::visitCompareNode(CompareNode& node)
{
  std::optional<int> lhs = node.left()->accept(Data<int>(), *this);
  std::optional<int> rhs = node.right()->accept(Data<int>(), *this);
  if (!lhs || !rhs) { return {}; }

  return node.doing(*lhs, *rhs);
}

template<typename T> std::optional<T> AstNodeVisitor::visitValueNode(ValueNode<T>& t_node) { return t_node.doing(); }

class SyntaxAnalysis
{
private:
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP