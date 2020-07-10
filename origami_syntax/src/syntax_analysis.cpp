/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Семантический парсер для создания абстрактного синтаксического дерева на основе context-free grammar
 ******************************************************************************************************************************************/

#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"
#include "origami_syntax/parsing/detail/utility.hpp"

namespace origami::parser {

SyntaxAnalyzerCpp::SyntaxAnalyzerCpp(const std::string& t_code)
{
  m_tokenizer.update(t_code);
  m_current_token = m_tokenizer.getToken();
}

std::shared_ptr<ast::AstNode> SyntaxAnalyzerCpp::factor()
{
  std::shared_ptr<ast::AstNode> node;

  switch (auto [token, lexeme] = m_current_token; token) {
    case lex::Token::Literal: {
      switch (utility::isNumber(lexeme)) {
        case utility::Number::Integer: {
          node = std::make_shared<ast::AstNodeNumber>(std::make_any<int>(std::stoi(lexeme)));
          break;
        }
        case utility::Number::Double: {
          node = std::make_shared<ast::AstNodeNumber>(std::make_any<double>(std::stod(lexeme)));
          break;
        }
        case utility::Number::Unknown: {
          throw InvalidSyntaxError{ "Data type casting error: " + lexeme };
        }
      }

      m_current_token = m_tokenizer.getToken();

      break;
    }
    case lex::Token::Punctuator: {
      if (lexeme == "(") {
        m_current_token = m_tokenizer.getToken();
        node = expr();
        if (m_current_token.second != ")") {
          throw InvalidSyntaxError{ "There is no closing bracket: " + m_current_token.second };
        }
      }

      m_current_token = m_tokenizer.getToken();

      break;
    }
    default: {
      throw InvalidSyntaxError{ "The data type isn't literal: " + lexeme };
    }
  }

  return node;
}

std::shared_ptr<ast::AstNode> SyntaxAnalyzerCpp::term()
{
  std::shared_ptr<ast::AstNode> tree = factor();

  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "*") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstNodeMathOperator>("*", tree, factor());
    } else if (m_current_token.second == "/") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstNodeMathOperator>("/", tree, factor());
    } else {
      break;
    }
  }

  return tree;
}

std::shared_ptr<ast::AstNode> SyntaxAnalyzerCpp::expr()
{
  std::shared_ptr<ast::AstNode> tree = term();

  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "+") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstNodeMathOperator>("+", tree, term());
    } else if (m_current_token.second == "-") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstNodeMathOperator>("-", tree, term());
    } else {
      break;
    }
  }

  return tree;
}

std::shared_ptr<ast::AstNode> SyntaxAnalyzerCpp::parse()
{
  return expr();
}

}// namespace origami::parser