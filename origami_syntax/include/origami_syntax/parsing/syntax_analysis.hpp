/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYNTAX_ANALYSIS_HPP
#define ORIGAMI_SYNTAX_ANALYSIS_HPP

#include "origami_lexical/lexical_analyzer/lexical_analyzer.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_cpp.hpp"
#include "origami_lexical/conventions/tokens.hpp"

#include <map>

namespace origami::parser {

class Terminal
{
};

class Operation : public Terminal
{
public:
  std::shared_ptr<Terminal> left;
  std::pair<lex::Token, lex::Lexeme> token;
  std::shared_ptr<Terminal> right;
};

class Numeric : public Terminal
{
public:
  std::pair<lex::Token, lex::Lexeme> token;
};

class SyntaxAnalysis
{
public:
  Terminal factor()
  {
  /// https://ruslanspivak.com/lsbasi-part7/
  }

private:
  lex::LexicalAnalyzer<lex::LexicalConventionCpp> m_tokenizer;
};
}// namespace origami::parser

#endif// ORIGAMI_SYNTAX_ANALYSIS_HPP