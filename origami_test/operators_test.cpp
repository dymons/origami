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

    origami::lex::LexicalConventions m_tokenizer; ///< Лексический анализатор
};

/**
  * \brief          Проверка определения всех символов "<", "<:", "<%", "<=", "<=>", "<<", "<<="
  */
TEST_F(OpeartorsTest, LessThanSign)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("<", {{ Token::Punctuator, "<" }}));
  ASSERT_TRUE(equalTokens("<:", {{ Token::Punctuator, "<:" }}));
  ASSERT_TRUE(equalTokens("<%", {{ Token::Punctuator, "<%" }}));
  ASSERT_TRUE(equalTokens("<=", {{ Token::Punctuator, "<=" }}));
  ASSERT_TRUE(equalTokens("<=>", {{ Token::Punctuator, "<=>" }}));
  ASSERT_TRUE(equalTokens("<<", {{ Token::Punctuator, "<<" }}));
  ASSERT_TRUE(equalTokens("<<=", {{ Token::Punctuator, "<<=" }}));

  // Если между операторами находится пробел - это ошибка
  ASSERT_TRUE(equalTokens("< :",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, ":" }}));
  ASSERT_TRUE(equalTokens("< %",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, "%" }}));
  ASSERT_TRUE(equalTokens("< =",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, "=" }}));
  ASSERT_TRUE(equalTokens("< =>",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, "=" },
                           { Token::Punctuator, ">" }}));
  ASSERT_TRUE(equalTokens("< <",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, "<" }}));
  ASSERT_TRUE(equalTokens("< <=",
                          {{ Token::Punctuator, "<" },
                           { Token::Punctuator, "<=" }}));

  ASSERT_TRUE(equalTokens("bool less = 50 < 60;",
                          {{ Token::Keyword,    "bool" },
                           { Token::Identifier, "less" },
                           { Token::Punctuator, "=" },
                           { Token::Literal,    "50" },
                           { Token::Punctuator, "<" },
                           { Token::Literal,    "60" },
                           { Token::Punctuator, ";" }}));
}