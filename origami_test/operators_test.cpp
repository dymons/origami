#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class OpeartorsTest : public ::testing::Test {
  protected:
    origami::lex::LexicalConventions m_tokenizer;
};

TEST_F(OpeartorsTest, LessThanSign)
{
  {
    const auto tokens = m_tokenizer.getTokens("<");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "<" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}