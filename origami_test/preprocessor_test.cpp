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
                           { Token::Operator,   "<" },
                           { Token::Identifier, "memory" },
                           { Token::Operator,   ">" }}));

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

TEST_F(PreprocessorsTest, If)
{
  using origami::lex::Token;
//
//  ASSERT_TRUE(equalTokens("#if", {{ Token::KeywordPreprocessor, "#if" }}));
//  ASSERT_TRUE(equalTokens("#elif ", {{ Token::KeywordPreprocessor, "#elif" }}));
//  ASSERT_TRUE(equalTokens("#else ", {{ Token::KeywordPreprocessor, "#else" }}));
//  ASSERT_TRUE(equalTokens("#endif ", {{ Token::KeywordPreprocessor, "#endif" }}));

  const std::string code = "#if DLEVEL > 5\n"
                           "    #define SIGNAL  1\n"
                           "    #if STACKUSE == 1\n"
                           "        #define STACK   200\n"
                           "    #else\n"
                           "        #define STACK   100\n"
                           "    #endif\n"
                           "#else\n"
                           "    #define SIGNAL  0\n"
                           "    #if STACKUSE == 1\n"
                           "        #define STACK   100\n"
                           "    #else\n"
                           "        #define STACK   50\n"
                           "    #endif\n"
                           "#endif\n"
                           "#if DLEVEL == 0\n"
                           "    #define STACK 0\n"
                           "#elif DLEVEL == 1\n"
                           "    #define STACK 100\n"
                           "#elif DLEVEL > 5\n"
                           "    display( debugptr );\n"
                           "#else\n"
                           "    #define STACK 200\n"
                           "#endif";

  EXPECT_EQ(countOfTokens(code, Token::KeywordPreprocessor), 23);
  ASSERT_TRUE(equalTokens(code,
                          {{ Token::KeywordPreprocessor, "#if" },
                           { Token::Identifier,          "DLEVEL" },
                           { Token::Operator,            ">" },
                           { Token::Literal,             "5" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "SIGNAL" },
                           { Token::Literal,             "1" },
                           { Token::KeywordPreprocessor, "#if" },
                           { Token::Identifier,          "STACKUSE" },
                           { Token::Operator,            "==" },
                           { Token::Literal,             "1" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "200" },
                           { Token::KeywordPreprocessor, "#else" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "100" },
                           { Token::KeywordPreprocessor, "#endif" },
                           { Token::KeywordPreprocessor, "#else" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "SIGNAL" },
                           { Token::Literal,             "0" },
                           { Token::KeywordPreprocessor, "#if" },
                           { Token::Identifier,          "STACKUSE" },
                           { Token::Operator,            "==" },
                           { Token::Literal,             "1" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "100" },
                           { Token::KeywordPreprocessor, "#else" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "50" },
                           { Token::KeywordPreprocessor, "#endif" },
                           { Token::KeywordPreprocessor, "#endif" },
                           { Token::KeywordPreprocessor, "#if" },
                           { Token::Identifier,          "DLEVEL" },
                           { Token::Operator,            "==" },
                           { Token::Literal,             "0" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "0" },
                           { Token::KeywordPreprocessor, "#elif" },
                           { Token::Identifier,          "DLEVEL" },
                           { Token::Operator,            "==" },
                           { Token::Literal,             "1" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "100" },
                           { Token::KeywordPreprocessor, "#elif" },
                           { Token::Identifier,          "DLEVEL" },
                           { Token::Operator,            ">" },
                           { Token::Literal,             "5" },
                           { Token::Identifier,          "display" },
                           { Token::Punctuator,          "(" },
                           { Token::Identifier,          "debugptr" },
                           { Token::Punctuator,          ")" },
                           { Token::Punctuator,          ";" },
                           { Token::KeywordPreprocessor, "#else" },
                           { Token::KeywordPreprocessor, "#define" },
                           { Token::Identifier,          "STACK" },
                           { Token::Literal,             "200" },
                           { Token::KeywordPreprocessor, "#endif" }
                          }));
}