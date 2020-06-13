#include "origami_lexical/symbol_table/symbol_table_cpp.hpp"

namespace origami::lex {

const std::set<std::string>& SymbolTableCpp::keywords()
{
  static const std::set<std::string> Keys{ "alignas",
    "alignof",
    "asm",
    "auto",
    "bool",
    "break",
    "case",
    "catch",
    "char",
    "char16_t",
    "char32_t",
    "class",
    "concept",
    "const",
    "constexpr",
    "const_cast",
    "continue",
    "decltype",
    "default",
    "delete",
    "do",
    "double",
    "dynamic_cast",
    "else",
    "enum",
    "explicit",
    "export",
    "extern",
    "false",
    "float",
    "for",
    "friend",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "mutable",
    "namespace",
    "new",
    "noexcept",
    "nullptr",
    "operator",
    "private",
    "protected",
    "public",
    "register",
    "reinterpret_cast",
    "requires",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "static_assert",
    "static_cast",
    "switch",
    "template",
    "this",
    "thread_local",
    "throw",
    "true",
    "try",
    "typedef",
    "typeid",
    "typename",
    "union",
    "unsigned",
    "using",
    "virtual",
    "void",
    "volatile",
    "wchar_t",
    "while",

    // Стандарт кодирования 5.12
    "and",
    "or",
    "xor",
    "not",
    "bitand",
    "bitor",
    "compl",
    "and_eq",
    "or_eq",
    "xor_eq",
    "not_eq" };

  return Keys;
}
const std::map<char, std::set<std::string>>& SymbolTableCpp::operators()
{
  // Стандарт кодирования 5.12
  static const std::map<char, std::set<std::string>> Op{ { '+', { "+", "+=", "++" } },
    { '-', { "-", "-=", "--" } },
    { '*', { "*", "*=" } },
    { '/', { "/", "/=" } },
    { '%', { "%", "%=" } },
    { '=', { "=", "==" } },
    { '!', { "!", "!=" } },
    { '>', { ">", ">=", ">>", ">>=" } },
    { '<', { "<", "<=", "<=>", "<<", "<<=" } },
    { '&', { "&", "&=", "&&" } },
    { '|', { "|", "|=", "||" } },
    { '^', { "^", "^=" } },
    { '?', { "?" } },
    { '~', { "~" } } };

  return Op;
}
const std::map<char, std::set<std::string>>& SymbolTableCpp::punctuation()
{
  // Стандарт кодирования 5.12
  static const std::map<char, std::set<std::string>> Punctuation{ { '<', { "<:", "<%" } },
    { '%', { "%>", "%:", "%:%" } },
    { '-', { "->", "->*" } },
    { '{', { "{" } },
    { '}', { "}" } },
    { '[', { "[" } },
    { ']', { "]" } },
    { '(', { "(" } },
    { ')', { ")" } },
    { ':', { ":", ":>", "::" } },
    { ';', { ";" } },
    { ';', { ";" } },
    { '.', { ".", "...", ".*" } },
    { ',', { "," } } };

  return Punctuation;
}
}// namespace origami::lex