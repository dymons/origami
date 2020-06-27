#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"
#include "origami_syntax/parsing/detail/utility.hpp"

#include <fmt/core.h>

namespace origami::parser {
AstNode::AstNode(const std::shared_ptr<AstNode>& t_left, const std::shared_ptr<AstNode>& t_right)
{
  m_left = t_left;
  m_right = t_right;
}
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
  } else if ((lhs.type() == typeid(double)) && (rhs.type() == typeid(double))) {
    return t_node.doing(std::any_cast<double>(lhs), std::any_cast<double>(rhs));
  } else {
    throw UnsupportedOperationError{ fmt::format("Для типов {0} и {1} не заданы правила обработки.", lhs.type().name(), rhs.type().name()) };
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
      switch (utility::isNumber(lexeme)) {
        case utility::Number::Integer: {
          node = std::make_any<int>(std::stoi(lexeme));
          break;
        }
        case utility::Number::Double: {
          node = std::make_any<double>(std::stod(lexeme));
          break;
        }
        case utility::Number::Unknown: {
          throw InvalidSyntaxError{"Data type casting error: " + lexeme};
        }
      }

      m_current_token = m_tokenizer.getToken();

      break;
    }
    default: {
      throw InvalidSyntaxError{"The data type isn't literal: " + lexeme};
    }
  }

  return node;
}

std::shared_ptr<AstNode> SyntaxAnalyzerCpp::expr()
{
  std::shared_ptr<AstNode> tree = std::make_shared<origami::parser::AstNodeNumber>(factor());

  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "+") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<origami::parser::AstNodeAdder>(tree, std::make_shared<origami::parser::AstNodeNumber>(factor()));
    } else {
      throw UnsupportedOperationError{"The parser doesn't support an operator " + m_current_token.second};
    }
  }

  return tree;
}

std::shared_ptr<AstNode> SyntaxAnalyzerCpp::parse()
{
  return expr();
}

}// namespace origami::parser