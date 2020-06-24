#include "origami_lexical/lexical_analyzer/lexical_analyzer.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_cpp.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_python.hpp"

#include <catch2/catch.hpp>

/**
 * \brief
 *
 * \param[in]
 * \param[in]
 *
 * \return
 */
template<typename T, typename U>
bool operator==(const std::pair<origami::lex::Token, T>& t_lhs, const std::pair<origami::lex::Token, U>& t_rhs)
{
  return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
}

TEST_CASE("Проверка аналитического разбора программы С++ на распознование лексем", "[tokenizer-cpp]")
{
  origami::lex::LexicalAnalyzer<origami::lex::LexicalConventionCpp> tokenizer;

  using origami::lex::Token;
  REQUIRE(tokenizer.empty());
  REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });

  SECTION("Проверка методов класса origami::lex::LexicalAnalyzer")
  {
    tokenizer.update("              ");
    REQUIRE(!tokenizer.empty());
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("              ");
    REQUIRE(!tokenizer.empty());
    tokenizer.clear();
    REQUIRE(tokenizer.empty());
  }
  SECTION("Проверка определения символов <, <:, <%, <=, <=>, <<, <<=")
  {
    tokenizer.update("< <: <% <= <=> << <<=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<:" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<=>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("< < : < % < = < => < < < <=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<:" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ":" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "%" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<=>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "<<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update(
      "auto main() -> int\n"
      "<%\n"
      "    auto lambda = <:&:> { };\n"
      "    constexpr auto a = 5;\n"
      "    constexpr auto b = 10;\n"
      "    static_assert(a < b);\n"
      "    static_assert(a <= b);\n"
      "\n"
      "    std::deque<int> container;\n"
      "    int value = 123;\n"
      "    do\n"
      "    <% \n"
      "        container.push_front(value);\n"
      "    %> while (value <<= 1);\n"
      "    return 0;\n"
      "%>");

    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "main" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "->" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "lambda" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<:" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ":>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "{" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "}" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "5" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "static_assert" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "static_assert" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "std" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "::" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "deque" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "container" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "123" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "do" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "container" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "." });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "push_front" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "%>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "while" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "1" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "0" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "%>" });
  }
  SECTION("Проверка определения символов >, >=, >>,  >>=")
  {
    tokenizer.update("> >= >>  >>=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">>=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("> > = > >  >> =");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, ">=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, ">>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, ">>=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });

    tokenizer.update(
      "auto main() -> int\n"
      "<%\n"
      "    constexpr auto a = 5;\n"
      "    constexpr auto b = 10;\n"
      "    static_assert(a > b);\n"
      "    static_assert(a >= b);\n"
      "\n"
      "    std::deque<bool> container;\n"
      "    int value = 123;\n"
      "    do\n"
      "    <% \n"
      "        container.push_front(value & 1);\n"
      "    %> while (value >>= 1);\n"
      "    return 0;\n"
      "%>");

    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "main" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "->" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "5" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "static_assert" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "static_assert" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "std" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "::" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "deque" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "bool" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "container" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "123" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "do" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "container" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "." });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "push_front" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "1" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "%>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "while" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "value" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">>=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "1" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "0" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "%>" });
  }
  SECTION("Проверка определения символов =, ==")
  {
    tokenizer.update("= ==");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "==" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("= =");
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "==" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update(
      "auto main() -> int\n"
      "{\n"
      "    constexpr auto a = 10;\n"
      "    constexpr auto b = 10;\n"
      "    static_assert(a == b);\n"
      "    return 0;\n"
      "}");

    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "main" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "->" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "{" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "constexpr" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "static_assert" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "a" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "==" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "b" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "0" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "}" });
  }
  SECTION("Проверка определения символов !, !=")
  {
    tokenizer.update("! !=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("! =");
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "!=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update(
      "auto main() -> int\n"
      "{\n"
      "    bool isFirst = 100 < 200;\n"
      "    if (!isFirst || (100 != 200))\n"
      "    {\n"
      "        return 1;\n"
      "    }\n"
      "\n"
      "    return 0;\n"
      "}");

    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "main" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "->" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "{" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "bool" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "isFirst" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "100" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "200" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "if" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "isFirst" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "||" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "100" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "200" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "{" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "1" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "}" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "0" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "}" });
  }
  SECTION("Проверка определения символов +, -, *, /, %, ++, --, +=, -=, *=, /=, %=")
  {
    tokenizer.update("+ - * / % ++ -- += -= *= /= %=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "+" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "-" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "*" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "/" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "++" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "--" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "+=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "-=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "*=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "/=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "%=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());

    tokenizer.update("+ + - - + = - = * = / = % =");
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "++" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "+" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "--" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "-" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "+=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "-=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "*=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "/=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE_FALSE(tokenizer.getToken() == std::pair{ Token::Operator, "%=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });


    tokenizer.update(
      "auto main() -> int\n"
      "{\n"
      "    int foo = 100;\n"
      "    int bar = 200;\n"
      "    foo = bar + 10;\n"
      "    bar = foo - 20;\n"
      "    foo = bar * 2;\n"
      "    bar = foo / 20;\n"
      "    bar = foo % 2;\n"
      "    bar++;\n"
      "    ++bar;\n"
      "    foo--;\n"
      "    --foo;\n"
      "    foo += 10;\n"
      "    bar -= 20;\n"
      "    foo *= 2;\n"
      "    bar /= 20;\n"
      "    bar %= 2;\n"
      "    return 0;\n"
      "}");

    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "auto" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "main" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "(" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ")" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "->" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "{" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "100" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "int" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "200" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "+" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "-" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "20" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "*" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "2" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "/" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "20" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "%" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "2" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "++" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "++" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "--" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "--" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "+=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "10" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "-=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "20" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "foo" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "*=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "2" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "/=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "20" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Identifier, "bar" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "%=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "2" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Keyword, "return" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Literal, "0" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, ";" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "}" });
  }
  SECTION("Проверка определения символов &, &=, &&, |, |=, ||, !, !=")
  {
    tokenizer.update("& &= && | |= || ! !=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&&" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "|" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "|=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "||" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "!=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());
  }
  SECTION("Проверка определения символов &, |, ^, ~, <<, >>")
  {
    tokenizer.update("& | ^ ~ << >>");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "&" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "|" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "^" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "~" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, ">>" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());
  }
}

TEST_CASE("Проверка аналитического разбора программы Python на распознование лексем", "[tokenizer-python]")
{
  origami::lex::LexicalAnalyzer<origami::lex::LexicalConventionPython> tokenizer;

  using origami::lex::Token;
  SECTION("Проверка определения символов <, <=, <<, <<=")
  {
    tokenizer.update("< <= << <<=");
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Operator, "<<" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Punctuator, "<<=" });
    REQUIRE(tokenizer.getToken() == std::pair{ Token::Eof, "" });
    REQUIRE(tokenizer.empty());
  }
}