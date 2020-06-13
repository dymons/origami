#include "origami_lexical/tokenizers/lexical_conventions.hpp"
#include "origami_lexical/symbol_table/symbol_table_cpp.hpp"

#include <catch2/catch.hpp>

/**
 * \brief          Преобразует исходный код программы в последовательность token'ов и сравнивает с ожидаемыми token'ами
 *
 * \param[in]      t_tokens - последовательность лексем, от лексического анализатора, для исходный кода программы
 * \param[in]      t_expect_tokens - ожидаемая последовательность token'ов, которые будут получены после лексического анализа
 *
 * \retval         true - Последовательность token'ов исходного кода программы совпадает с ожидаемой последовательностью
 * \retval         false - Последовательность token'ов исходного кода программы не совпадает с ожидаемой последовательностью
 */
template<typename T> bool equalTokens(const T& t_tokens, const T& t_expect_tokens)
{
  return std::equal(t_tokens.begin(), t_tokens.end(), t_expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
    return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
  });
}

/**
 * \brief          Подсчитывает количество token'ов, полученных после лексического анализа, указанного типа
 *
 * \param[in]      t_tokens - последовательность лексем, от лексического анализатора, для исходный кода программы
 * \param[in]      t_count_token - значение token'ов, количество которых необходимо посчитать после лексического анализа
 *
 * \return         Количество token'ов указанного типа
 */
template<typename T, typename... Tokens> auto countOfTokens(const T& t_tokens, Tokens... t_count_token) -> std::string::difference_type
{
  static_assert(sizeof...(t_count_token) > 0);
  return std::count_if(t_tokens.begin(), t_tokens.end(), [=](const auto& t_token) { return ((t_token.first == t_count_token) || ...); });
}

TEST_CASE("Проверка аналитического разбора программы С++ на распознование лексем", "[tokenizer-cpp]")
{
  origami::lex::LexicalConventions<origami::lex::SymbolTableCpp> tokenizer;

  using origami::lex::Token;
  SECTION("Проверка определения символов <, <:, <%, <=, <=>, <<, <<=")
  {
    REQUIRE(equalTokens(tokenizer.getTokens("<"), { { Token::Operator, "<" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<:"), { { Token::Punctuator, "<:" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<%"), { { Token::Punctuator, "<%" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<="), { { Token::Operator, "<=" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<=>"), { { Token::Operator, "<=>" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<<"), { { Token::Operator, "<<" } }));
    REQUIRE(equalTokens(tokenizer.getTokens("<<="), { { Token::Operator, "<<=" } }));

    REQUIRE(!equalTokens(tokenizer.getTokens("< :"), { { Token::Punctuator, "<:" } }));
    REQUIRE(!equalTokens(tokenizer.getTokens("< %"), { { Token::Punctuator, "<%" } }));
    REQUIRE(!equalTokens(tokenizer.getTokens("< ="), { { Token::Operator, "<=" } }));
    REQUIRE(!equalTokens(tokenizer.getTokens("< =>"), { { Token::Operator, "<=>" } }));
    REQUIRE(!equalTokens(tokenizer.getTokens("< <"), { { Token::Operator, "<<" } }));
    REQUIRE(!equalTokens(tokenizer.getTokens("< <="), { { Token::Operator, "<<=" } }));

    const std::string code =
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
      "%>";

    REQUIRE(countOfTokens(tokenizer.getTokens(code), Token::Operator) == 10);
    REQUIRE(countOfTokens(tokenizer.getTokens(code), Token::Punctuator) == 31);
    REQUIRE(equalTokens(tokenizer.getTokens(code),
      { { Token::Keyword, "auto" },
        { Token::Identifier, "main" },
        { Token::Punctuator, "(" },
        { Token::Punctuator, ")" },
        { Token::Punctuator, "->" },
        { Token::Keyword, "int" },
        { Token::Punctuator, "<%" },
        { Token::Keyword, "auto" },
        { Token::Identifier, "lambda" },
        { Token::Operator, "=" },
        { Token::Punctuator, "<:" },
        { Token::Operator, "&" },
        { Token::Punctuator, ":>" },
        { Token::Punctuator, "{" },
        { Token::Punctuator, "}" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "constexpr" },
        { Token::Keyword, "auto" },
        { Token::Identifier, "a" },
        { Token::Operator, "=" },
        { Token::Literal, "5" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "constexpr" },
        { Token::Keyword, "auto" },
        { Token::Identifier, "b" },
        { Token::Operator, "=" },
        { Token::Literal, "10" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "static_assert" },
        { Token::Punctuator, "(" },
        { Token::Identifier, "a" },
        { Token::Operator, "<" },
        { Token::Identifier, "b" },
        { Token::Punctuator, ")" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "static_assert" },
        { Token::Punctuator, "(" },
        { Token::Identifier, "a" },
        { Token::Operator, "<=" },
        { Token::Identifier, "b" },
        { Token::Punctuator, ")" },
        { Token::Punctuator, ";" },
        { Token::Identifier, "std" },
        { Token::Punctuator, "::" },
        { Token::Identifier, "deque" },
        { Token::Operator, "<" },
        { Token::Keyword, "int" },
        { Token::Operator, ">" },
        { Token::Identifier, "container" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "int" },
        { Token::Identifier, "value" },
        { Token::Operator, "=" },
        { Token::Literal, "123" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "do" },
        { Token::Punctuator, "<%" },
        { Token::Identifier, "container" },
        { Token::Punctuator, "." },
        { Token::Identifier, "push_front" },
        { Token::Punctuator, "(" },
        { Token::Identifier, "value" },
        { Token::Punctuator, ")" },
        { Token::Punctuator, ";" },
        { Token::Punctuator, "%>" },
        { Token::Keyword, "while" },
        { Token::Punctuator, "(" },
        { Token::Identifier, "value" },
        { Token::Operator, "<<=" },
        { Token::Literal, "1" },
        { Token::Punctuator, ")" },
        { Token::Punctuator, ";" },
        { Token::Keyword, "return" },
        { Token::Literal, "0" },
        { Token::Punctuator, ";" },
        { Token::Punctuator, "%>" } }));
  }
}