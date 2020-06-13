#include "origami_lexical/tokenizers/lexical_conventions.hpp"
#include "origami_lexical/symbol_table/symbol_table_cpp.hpp"

#include <catch2/catch.hpp>

/**
 * \brief          Преобразует исходный код программы в последовательность token'ов и сравнивает с ожидаемыми token'ами
 *
 * \param[in]      t_tokens - последовательность лексем ло лексического анализатора исходный код программы
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
 * \param[in]      t_code - исходный код программы
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
  }
}