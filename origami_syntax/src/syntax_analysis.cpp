/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Семантический парсер для создания абстрактного синтаксического дерева на основе context-free grammar
 ******************************************************************************************************************************************/

#include "origami_syntax/parsing/syntax_analysis.hpp"
#include "origami_lexical/conventions/exeptions.hpp"
#include "origami_syntax/parsing/detail/utility.hpp"

namespace origami::parser {

SyntaxAnalyzerCpp::SyntaxAnalyzerCpp(const std::string& t_code) : m_tokenizer{ t_code }
{
  m_current_token = m_tokenizer.getToken();
}

std::shared_ptr<ast::AstBase> SyntaxAnalyzerCpp::factor()
{
  std::shared_ptr<ast::AstBase> node;

  switch (auto [token, lexeme] = m_current_token; token) {
    // (PLUS | MINUS) factor
    case lex::Token::Operator: {
      switch (const auto hash = utility::fnv1a::hash(lexeme); hash) {
        case utility::fnv1a::hash("+"): {
          m_current_token = m_tokenizer.getToken();
          node = std::make_shared<ast::AstUnaryOperator>("+", factor());
          break;
        }
        case utility::fnv1a::hash("-"): {
          m_current_token = m_tokenizer.getToken();
          node = std::make_shared<ast::AstUnaryOperator>("-", factor());
          break;
        }
        default: {
          throw InvalidSyntaxError{ "Неподдерживаемая унарная операция: " + lexeme };
        }
      }

      break;
    }
    // (INTEGER | DOUBLE)
    case lex::Token::Literal: {
      switch (utility::isNumber(lexeme)) {
        case utility::Number::Integer: {
          node = std::make_shared<ast::AstNumber>(std::make_any<int>(std::stoi(lexeme)));
          break;
        }
        case utility::Number::Double: {
          node = std::make_shared<ast::AstNumber>(std::make_any<double>(std::stod(lexeme)));
          break;
        }
        default: {
          throw InvalidSyntaxError{ "Неподдерживаемый тип данных для " + lexeme };
        }
      }

      m_current_token = m_tokenizer.getToken();

      break;
    }
    // LPARAM expr RPARAM
    case lex::Token::Punctuator: {
      if (lexeme == "(") {
        m_current_token = m_tokenizer.getToken();
        node = expr();
        if (m_current_token.second != ")") {
          throw InvalidSyntaxError{ "Нет закрывающей скобки ')': " + m_current_token.second };
        }
      }

      m_current_token = m_tokenizer.getToken();

      break;
    }
    default: {
      throw InvalidSyntaxError{ "Данные не относятся к категории lexeme : " + lexeme };
    }
  }

  return node;
}

std::shared_ptr<ast::AstBase> SyntaxAnalyzerCpp::term()
{
  std::shared_ptr<ast::AstBase> tree = factor();

  // factor ((MUL | DIV) factor)*
  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "*") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstMathOperator>("*", tree, factor());
    } else if (m_current_token.second == "/") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstMathOperator>("/", tree, factor());
    } else {
      break;
    }
  }

  return tree;
}

std::shared_ptr<ast::AstBase> SyntaxAnalyzerCpp::expr()
{
  std::shared_ptr<ast::AstBase> tree = term();

  // term ((PLUS | MINUS) term)*
  while (m_current_token.first == lex::Token::Operator) {
    if (m_current_token.second == "+") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstMathOperator>("+", tree, term());
    } else if (m_current_token.second == "-") {
      m_current_token = m_tokenizer.getToken();
      tree = std::make_shared<ast::AstMathOperator>("-", tree, term());
    } else {
      break;
    }
  }

  return tree;
}

std::shared_ptr<ast::AstBase> SyntaxAnalyzerCpp::parse()
{
  return expr();
}

}// namespace origami::parser