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

#include <any>

namespace origami::parser {

class SyntaxAnalyzerCpp
{
public:
  explicit SyntaxAnalyzerCpp(const std::string& t_code);

  std::shared_ptr<ast::AstNode> parse();

private:
  std::shared_ptr<ast::AstNode> factor();

  std::shared_ptr<ast::AstNode> expr();

  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;

  std::pair<lex::Token, lex::Lexeme> m_current_token;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP