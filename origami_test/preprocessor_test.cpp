#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class PreprocessorsTest : public ::testing::Test {
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
  * \brief          Проверка предпроцессора #include
  */
TEST_F(PreprocessorsTest, Include)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("#", {{ Token::Punctuator, "#" }}));

  ASSERT_TRUE(equalTokens("# include",
                          {{ Token::KeywordPreprocessor, "#include" }}));

  ASSERT_TRUE(equalTokens("# include ",
                          {{ Token::KeywordPreprocessor, "#include" }}));

  ASSERT_TRUE(equalTokens("# + include",
                          {{ Token::Punctuator, "#" },
                           { Token::Operator,   "+" },
                           { Token::Identifier, "include" }}));

  ASSERT_TRUE(equalTokens("# include <memory>",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "<memory>" }}));

  ASSERT_TRUE(equalTokens("# include \"memory\"",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "\"memory\"" }}));

  ASSERT_TRUE(equalTokens("# + include \"memory\"",
                          {{ Token::Punctuator, "#" },
                           { Token::Operator,   "+" },
                           { Token::Identifier, "include" },
                           { Token::Literal,    "\"memory\"" }}));

  ASSERT_TRUE(equalTokens("# include + \"memory\"",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Operator,            "+" },
                           { Token::Literal,             "\"memory\"" }}));

  ASSERT_TRUE(equalTokens("# include int \"memory\"",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Keyword,             "int" },
                           { Token::Literal,             "\"memory\"" }}));

  ASSERT_TRUE(equalTokens("# include 0.0123 \"memory\"",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "0.0123" },
                           { Token::Literal,             "\"memory\"" }}));

  ASSERT_TRUE(equalTokens("# include <memory>\n # include \"algorithm\" ",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "<memory>" },
                           { Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "\"algorithm\"" }}));

  ASSERT_TRUE(equalTokens("#include <memory>\n# include \"origami_lexical/tokens.hpp\"\n int main()\n{\n\treturn 0;\n}",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "<memory>" },
                           { Token::KeywordPreprocessor, "#include" },
                           { Token::Literal,             "\"origami_lexical/tokens.hpp\"" },
                           { Token::Keyword,             "int" },
                           { Token::Identifier,          "main" },
                           { Token::Punctuator,          "(" },
                           { Token::Punctuator,          ")" },
                           { Token::Punctuator,          "{" },
                           { Token::Keyword,             "return" },
                           { Token::Literal,             "0" },
                           { Token::Punctuator,          ";" },
                           { Token::Punctuator,          "}" }}));

  ASSERT_TRUE(equalTokens("# icustomdefine",
                          {{ Token::Punctuator, "#" },
                           { Token::Identifier, "icustomdefine" }}));

  ASSERT_TRUE(equalTokens("# icustomdefine <memory>",
                          {{ Token::Punctuator, "#" },
                           { Token::Identifier, "icustomdefine" },
                           { Token::Operator, "<" },
                           { Token::Identifier, "memory" },
                           { Token::Operator, ">" }}));

  ASSERT_TRUE(equalTokens("# includecustomdefine",
                          {{ Token::Punctuator, "#" },
                           { Token::Identifier, "includecustomdefine" }}));

  ASSERT_TRUE(equalTokens("# include customdefine",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Identifier,          "customdefine" }}));

  ASSERT_TRUE(equalTokens("# include \"customdefine",
                          {{ Token::KeywordPreprocessor, "#include" },
                           { Token::Punctuator,          "\"" },
                           { Token::Identifier,          "customdefine" }}));
}