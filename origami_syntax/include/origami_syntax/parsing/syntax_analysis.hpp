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
#include "origami_syntax/parsing/abstract_syntax_tree.hpp"

namespace origami::parser {

class SyntaxAnalyzerCpp
{
public:
  explicit SyntaxAnalyzerCpp(const std::string& t_code);

  std::shared_ptr<ast::AstBase> parse();

private:
  /**
   * \brief
   *
   * \details   (PLUS | MINUS) factor | (INTEGER | DOUBLE) | LPARAM expr RPARAM
   */
  std::shared_ptr<ast::AstBase> factor();

  /**
   * \brief
   *
   * \details   factor ((MUL | DIV) factor)*
   */
  std::shared_ptr<ast::AstBase> term();

  /**
   * \brief
   *
   * \details   term ((PLUS | MINUS) term)*
   */
  std::shared_ptr<ast::AstBase> expr();

private:
  /// Лексический анализатор
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;

  /// Текущий токен
  std::pair<lex::Token, lex::Lexeme> m_current_token;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP