#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class OpeartorsTest : public ::testing::Test {
  protected:
    /**
      * \brief          Преобразует исходный код программы в последовательность token'ов и сравнивает с ожидаемыми token'ами
      *
      * \param[in]      t_code - исходный код программы
      * \param[in]      t_expect_tokens - ожидаемая последовательность token'ов, которые будут получены после лексического анализа
      *
      * \retval         true - Последовательность token'ов исходного кода программы совпадает с ожидаемой последовательностью
      * \retval         false - Последовательность token'ов исходного кода программы не совпадает с ожидаемой последовательностью
      */
    bool equalTokens(const std::string& t_code, std::vector<std::pair<origami::lex::Token, std::string>> t_expect_tokens)
    {
      const auto tokens = m_tokenizer.getTokens(t_code);
      return std::equal(tokens.begin(), tokens.end(), t_expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
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
    template<typename ... Tokens>
    std::size_t countOfTokens(const std::string& t_code, Tokens ... t_count_token)
    {
      const auto tokens = m_tokenizer.getTokens(t_code);
      return std::count_if(tokens.begin(), tokens.end(), [=](const auto& t_token) {
        return ((t_token.first == t_count_token) || ...);
      });
    }

    origami::lex::LexicalConventions m_tokenizer; ///< Лексический анализатор
};

/**
  * \brief          Проверка определения символов "<", "<:", "<%", "<=", "<=>", "<<", "<<="
  */
TEST_F(OpeartorsTest, LessThanSign)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("<", {{ Token::Operator, "<" }}));
  ASSERT_TRUE(equalTokens("<:", {{ Token::Punctuator, "<:" }}));
  ASSERT_TRUE(equalTokens("<%", {{ Token::Punctuator, "<%" }}));
  ASSERT_TRUE(equalTokens("<=", {{ Token::Operator, "<=" }}));
  ASSERT_TRUE(equalTokens("<=>", {{ Token::Operator, "<=>" }}));
  ASSERT_TRUE(equalTokens("<<", {{ Token::Operator, "<<" }}));
  ASSERT_TRUE(equalTokens("<<=", {{ Token::Operator, "<<=" }}));

  // Если между операторами находится пробел - это ошибка
  ASSERT_TRUE(equalTokens("< :",
                          {{ Token::Operator,   "<" },
                           { Token::Punctuator, ":" }}));
  ASSERT_TRUE(equalTokens("< %",
                          {{ Token::Operator, "<" },
                           { Token::Operator, "%" }}));
  ASSERT_TRUE(equalTokens("< =",
                          {{ Token::Operator, "<" },
                           { Token::Operator, "=" }}));
  ASSERT_TRUE(equalTokens("< =>",
                          {{ Token::Operator, "<" },
                           { Token::Operator, "=" },
                           { Token::Operator, ">" }}));
  ASSERT_TRUE(equalTokens("< <",
                          {{ Token::Operator, "<" },
                           { Token::Operator, "<" }}));
  ASSERT_TRUE(equalTokens("< <=",
                          {{ Token::Operator, "<" },
                           { Token::Operator, "<=" }}));

  ASSERT_TRUE(equalTokens("bool less = 50 < 60;",
                          {{ Token::Keyword,    "bool" },
                           { Token::Identifier, "less" },
                           { Token::Operator,   "=" },
                           { Token::Literal,    "50" },
                           { Token::Operator,   "<" },
                           { Token::Literal,    "60" },
                           { Token::Punctuator, ";" }}));
}

/**
  * \brief          Проверка определения символов "=", "=="
  */
TEST_F(OpeartorsTest, EqualTo)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("=", {{ Token::Operator, "=" }}));
  ASSERT_TRUE(equalTokens("==", {{ Token::Operator, "==" }}));
  ASSERT_FALSE(equalTokens("= =", {{ Token::Operator, "==" }}));

  const std::string code = "auto main() -> int\n"
                           "{\n"
                           "    constexpr auto a = 10;\n"
                           "    constexpr auto b = 10;\n"
                           "    static_assert(a == b);\n"
                           "    return 0;\n"
                           "}";

  ASSERT_TRUE(equalTokens(code,
                          {{ Token::Keyword,    "auto" },
                           { Token::Identifier, "main" },
                           { Token::Punctuator, "(" },
                           { Token::Punctuator, ")" },
                           { Token::Punctuator, "->" },
                           { Token::Keyword,    "int" },
                           { Token::Punctuator, "{" },
                           { Token::Keyword,    "constexpr" },
                           { Token::Keyword,    "auto" },
                           { Token::Identifier, "a" },
                           { Token::Operator,   "=" },
                           { Token::Literal,    "10" },
                           { Token::Punctuator, ";" },
                           { Token::Keyword,    "constexpr" },
                           { Token::Keyword,    "auto" },
                           { Token::Identifier, "b" },
                           { Token::Operator,   "=" },
                           { Token::Literal,    "10" },
                           { Token::Punctuator, ";" },
                           { Token::Keyword,    "static_assert" },
                           { Token::Punctuator, "(" },
                           { Token::Identifier, "a" },
                           { Token::Operator,   "==" },
                           { Token::Identifier, "b" },
                           { Token::Punctuator, ")" },
                           { Token::Punctuator, ";" },
                           { Token::Keyword,    "return" },
                           { Token::Literal,    "0" },
                           { Token::Punctuator, ";" },
                           { Token::Punctuator, "}" }}));

  ASSERT_EQ(countOfTokens(code, Token::Operator), 3);
  ASSERT_EQ(countOfTokens(code, Token::Operator, Token::Punctuator), 14);
}