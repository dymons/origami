#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class OpeartorsTest : public ::testing::Test {
  protected:
    origami::lex::LexicalConventions m_tokenizer;
};

TEST_F(OpeartorsTest, LessThanSign)
{
  using origami::lex::Token;

  auto check_code = [this](const std::string& t_code, std::vector<std::pair<Token, std::string>> t_expect_tokens) -> bool {
    const auto tokens = m_tokenizer.getTokens(t_code);
    return std::equal(tokens.begin(), tokens.end(), t_expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    });
  };

  // TODO: cpp + yaml, описание примеров
  /*
   * operators
   * |-less_than
   *   |-cpp's + yaml's + test
   * */

  // Проверка определения всех символов "<", "<:", "<%", "<=", "<=>", "<<", "<<="
  ASSERT_TRUE(check_code("<", {{ Token::Punctuator, "<" }}));
  ASSERT_TRUE(check_code("<:", {{ Token::Punctuator, "<:" }}));
  ASSERT_TRUE(check_code("<%", {{ Token::Punctuator, "<%" }}));
  ASSERT_TRUE(check_code("<=", {{ Token::Punctuator, "<=" }}));
  ASSERT_TRUE(check_code("<=>", {{ Token::Punctuator, "<=>" }}));
  ASSERT_TRUE(check_code("<<", {{ Token::Punctuator, "<<" }}));
  ASSERT_TRUE(check_code("<<=", {{ Token::Punctuator, "<<=" }}));

  // Если между операторами находится пробел - это ошибка
  ASSERT_TRUE(check_code("< :",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, ":" }}));
  ASSERT_TRUE(check_code("< %",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, "%" }}));
  ASSERT_TRUE(check_code("< =",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, "=" }}));
  ASSERT_TRUE(check_code("< =>",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, "=" },
                          { Token::Punctuator, ">" }}));
  ASSERT_TRUE(check_code("< <",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, "<" }}));
  ASSERT_TRUE(check_code("< <=",
                         {{ Token::Punctuator, "<" },
                          { Token::Punctuator, "<=" }}));

  ASSERT_TRUE(check_code("bool less = 50 < 60;",
                         {{ Token::Keyword,    "bool" },
                          { Token::Identifier, "less" },
                          { Token::Punctuator, "=" },
                          { Token::Literal,    "50" },
                          { Token::Punctuator, "<" },
                          { Token::Literal,    "60" },
                          { Token::Punctuator, ";" }}));
}