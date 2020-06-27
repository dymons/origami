#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"

#include <fmt/core.h>

namespace origami::parser {
void AstNode::setLeftChild(const std::shared_ptr<AstNode>& t_child) { m_left = t_child; }
void AstNode::setRightChild(const std::shared_ptr<AstNode>& t_child) { m_right = t_child; };
std::shared_ptr<AstNode> AstNode::getLeftChild() const { return m_left; }
std::shared_ptr<AstNode> AstNode::getRightChild() const { return m_right; }

std::any AstVisitor::visit(AstNodeNumber& t_node) { return t_node.doing(); }
std::any AstVisitor::visit(AstNodeAdder& t_node)
{
  if (!t_node.getLeftChild() || !t_node.getRightChild()) { return {}; }

  const std::any lhs = t_node.getLeftChild()->accept(*this);
  const std::any rhs = t_node.getRightChild()->accept(*this);

  if (!lhs.has_value() || !rhs.has_value()) { return {}; }

  // TODO: Найти решение по работе с данными, посмотреть в сторону паттернов.
  if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<int>(rhs));
  } else if ((lhs.type() == typeid(int)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<int>(lhs), std::any_cast<double>(rhs));
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(int))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<int>(rhs));
  } else {
    throwUnsupportedOperator();
  }
}

SyntaxAnalyzerCpp::SyntaxAnalyzerCpp(const std::string& t_code)
{
  m_tokenizer.update(t_code);
  m_current_token = m_tokenizer.getToken();
}

std::any SyntaxAnalyzerCpp::factor()
{
  std::any node;

  switch (auto [token, lexeme] = m_current_token; token) {
    case lex::Token::Literal: {
      // https://stackoverflow.com/questions/29169153/how-do-i-verify-a-string-is-valid-double-even-if-it-has-a-point-in-it
      // TODO: Сделать проверку на число, и в зависимости от типа, создавать any.
      node = std::make_any<int>(std::stoi(lexeme));
      m_current_token = m_tokenizer.getToken();
      break;
    }
    default: {
      throwInvalidSyntax();
    }
  }

  return node;
}

std::shared_ptr<AstNode> SyntaxAnalyzerCpp::expr()
{
  std::shared_ptr<AstNode> tree = std::make_shared<origami::parser::AstNodeNumber>(factor());

  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "+") {
      auto node = std::make_shared<origami::parser::AstNodeAdder>();
      node->setLeftChild(tree);
      m_current_token = m_tokenizer.getToken();
      node->setRightChild(std::make_shared<origami::parser::AstNodeNumber>(factor()));
      tree = node;
    } else {
      throwInvalidSyntax();
    }
  }

  return tree;
}
}// namespace origami::parser