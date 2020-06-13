#include "origami_lexical/symbol_table/symbol_table_python.hpp"

namespace origami::lex {

const std::set<std::string>& SymbolTablePython::keywords()
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
const std::map<char, std::set<std::string>>& SymbolTablePython::operators()
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

const std::map<char, std::set<std::string>>& SymbolTablePython::punctuation()
{
  static const std::map<char, std::set<std::string>> Punctuation{ { '(', { "(" } },
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
                                                                  { '-', { "->","-=" } },
                                                                  { '+', { "+=" } },
                                                                  { '*', { "*=","**=" } },
                                                                  { '/', { "/=","//=" } },
                                                                  { '%', { "%=" } },
                                                                  { '&', { "&=" } },
                                                                  { '|', { "|=" } },
                                                                  { '^', { "^=" } },
                                                                  { '>', { ">>=" } },
                                                                  { '<', { "<<=" } },};

  return Punctuation;
}
}// namespace origami::lex