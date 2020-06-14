#include "origami_lexical/lexical_conventions/lexical_convention_python.hpp"

namespace origami::lex {

const std::set<std::string>& LexicalConventionPython::keywords()
{
  static const std::set<std::string> Keys{ "False",
    "None",
    "True",
    "and",
    "as",
    "assert",
    "async",
    "await",
    "break",
    "class",
    "continue",
    "def",
    "del",
    "elif",
    "except",
    "finally",
    "for",
    "from",
    "global",
    "if",
    "import",
    "in",
    "is",
    "lambda",
    "nonlocal",
    "not",
    "or",
    "pass",
    "raise",
    "return",
    "try",
    "while",
    "with",
    "yield" };
  return Keys;
}
const std::map<char, std::set<std::string>>& LexicalConventionPython::operators()
{
  static const std::map<char, std::set<std::string>> Op{ { '+', { "+" } },
    { '-', { "-" } },
    { '*', { "*", "**" } },
    { '/', { "/", "//" } },
    { '%', { "%" } },
    { '@', { "@" } },
    { '<', { "<", "<<", "<=" } },
    { '>', { ">", ">>", ">=" } },
    { '&', { "&" } },
    { '|', { "|" } },
    { '^', { "^" } },
    { '~', { "~" } },
    { ':', { ":=" } },
    { ':', { ":=" } },
    { '=', { "==" } },
    { '!', { "!=" } } };

  return Op;
}

const std::map<char, std::set<std::string>>& LexicalConventionPython::punctuation()
{
  static const std::map<char, std::set<std::string>> Punctuation{
    { '(', { "(" } },
    { ')', { ")" } },
    { '[', { "[" } },
    { ']', { "]" } },
    { '{', { "{" } },
    { '}', { "}" } },
    { ',', { "," } },
    { ':', { ":" } },
    { '.', { "." } },
    { ';', { ";" } },
    { '@', { "@", "@=" } },
    { '=', { "=" } },
    { '-', { "->", "-=" } },
    { '+', { "+=" } },
    { '*', { "*=", "**=" } },
    { '/', { "/=", "//=" } },
    { '%', { "%=" } },
    { '&', { "&=" } },
    { '|', { "|=" } },
    { '^', { "^=" } },
    { '>', { ">>=" } },
    { '<', { "<<=" } },
  };

  return Punctuation;
}
}// namespace origami::lex