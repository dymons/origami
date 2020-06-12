#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class OpeartorsTest : public ::testing::Test
{
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
  template<typename... Tokens> auto countOfTokens(const std::string& t_code, Tokens... t_count_token) -> std::string::difference_type
  {
    static_assert(sizeof...(t_count_token) > 0);
    const auto tokens = m_tokenizer.getTokens(t_code);
    return std::count_if(tokens.begin(), tokens.end(), [=](const auto& t_token) { return ((t_token.first == t_count_token) || ...); });
  }

  origami::lex::LexicalConventions m_tokenizer;///< Лексический анализатор
};

/**
 * \brief          Проверка определения символов "<", "<:", "<%", "<=", "<=>", "<<", "<<="
 */
TEST_F(OpeartorsTest, LessThanSign)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("<", { { Token::Operator, "<" } }));
  ASSERT_TRUE(equalTokens("<:", { { Token::Punctuator, "<:" } }));
  ASSERT_TRUE(equalTokens("<%", { { Token::Punctuator, "<%" } }));
  ASSERT_TRUE(equalTokens("<=", { { Token::Operator, "<=" } }));
  ASSERT_TRUE(equalTokens("<=>", { { Token::Operator, "<=>" } }));
  ASSERT_TRUE(equalTokens("<<", { { Token::Operator, "<<" } }));
  ASSERT_TRUE(equalTokens("<<=", { { Token::Operator, "<<=" } }));

  ASSERT_FALSE(equalTokens("< :", { { Token::Punctuator, "<:" } }));
  ASSERT_FALSE(equalTokens("< %", { { Token::Punctuator, "<%" } }));
  ASSERT_FALSE(equalTokens("< =", { { Token::Operator, "<=" } }));
  ASSERT_FALSE(equalTokens("< =>", { { Token::Operator, "<=>" } }));
  ASSERT_FALSE(equalTokens("< <", { { Token::Operator, "<<" } }));
  ASSERT_FALSE(equalTokens("< <=", { { Token::Operator, "<<=" } }));

  const std::string code =
    "#include <iostream>\n"
    "#include <deque>\n"
    "\n"
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

  EXPECT_EQ(countOfTokens(code, Token::Operator), 10);
  EXPECT_EQ(countOfTokens(code, Token::Punctuator), 31);
  ASSERT_TRUE(equalTokens(code,
    { { Token::KeywordPreprocessor, "#include" },
      { Token::Literal, "<iostream>" },
      { Token::KeywordPreprocessor, "#include" },
      { Token::Literal, "<deque>" },
      { Token::Keyword, "auto" },
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

/**
 * \brief          Проверка определения символов ">", ">=", ">>",  ">>="
 */
TEST_F(OpeartorsTest, GreaterThanSign)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens(">", { { Token::Operator, ">" } }));
  ASSERT_TRUE(equalTokens(">=", { { Token::Operator, ">=" } }));
  ASSERT_TRUE(equalTokens(">>", { { Token::Operator, ">>" } }));
  ASSERT_TRUE(equalTokens(">>=", { { Token::Operator, ">>=" } }));
  ASSERT_FALSE(equalTokens("> =", { { Token::Operator, ">=" } }));
  ASSERT_FALSE(equalTokens("> >", { { Token::Operator, ">>" } }));
  ASSERT_FALSE(equalTokens("> >=", { { Token::Operator, ">>=" } }));

  const std::string code =
    "#include <iostream>\n"
    "#include <deque>\n"
    "\n"
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
    "%>";

  EXPECT_EQ(countOfTokens(code, Token::Operator), 9);
  EXPECT_EQ(countOfTokens(code, Token::Punctuator), 26);
  ASSERT_TRUE(equalTokens(code,
    { { Token::KeywordPreprocessor, "#include" },
      { Token::Literal, "<iostream>" },
      { Token::KeywordPreprocessor, "#include" },
      { Token::Literal, "<deque>" },
      { Token::Keyword, "auto" },
      { Token::Identifier, "main" },
      { Token::Punctuator, "(" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, "->" },
      { Token::Keyword, "int" },
      { Token::Punctuator, "<%" },
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
      { Token::Operator, ">" },
      { Token::Identifier, "b" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "static_assert" },
      { Token::Punctuator, "(" },
      { Token::Identifier, "a" },
      { Token::Operator, ">=" },
      { Token::Identifier, "b" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "std" },
      { Token::Punctuator, "::" },
      { Token::Identifier, "deque" },
      { Token::Operator, "<" },
      { Token::Keyword, "bool" },
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
      { Token::Operator, "&" },
      { Token::Literal, "1" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "%>" },
      { Token::Keyword, "while" },
      { Token::Punctuator, "(" },
      { Token::Identifier, "value" },
      { Token::Operator, ">>=" },
      { Token::Literal, "1" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "return" },
      { Token::Literal, "0" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "%>" } }));
}

/**
 * \brief          Проверка определения символов "=", "=="
 */
TEST_F(OpeartorsTest, EqualTo)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("=", { { Token::Operator, "=" } }));
  ASSERT_TRUE(equalTokens("==", { { Token::Operator, "==" } }));
  ASSERT_FALSE(equalTokens("= =", { { Token::Operator, "==" } }));

  const std::string code =
    "auto main() -> int\n"
    "{\n"
    "    constexpr auto a = 10;\n"
    "    constexpr auto b = 10;\n"
    "    static_assert(a == b);\n"
    "    return 0;\n"
    "}";

  EXPECT_EQ(countOfTokens(code, Token::Operator), 3);
  EXPECT_EQ(countOfTokens(code, Token::Punctuator), 11);
  ASSERT_TRUE(equalTokens(code,
    { { Token::Keyword, "auto" },
      { Token::Identifier, "main" },
      { Token::Punctuator, "(" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, "->" },
      { Token::Keyword, "int" },
      { Token::Punctuator, "{" },
      { Token::Keyword, "constexpr" },
      { Token::Keyword, "auto" },
      { Token::Identifier, "a" },
      { Token::Operator, "=" },
      { Token::Literal, "10" },
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
      { Token::Operator, "==" },
      { Token::Identifier, "b" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "return" },
      { Token::Literal, "0" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "}" } }));
}

/**
 * \brief          Проверка определения символов "!", "!="
 */
TEST_F(OpeartorsTest, NotEqualTo)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("!", { { Token::Operator, "!" } }));
  ASSERT_TRUE(equalTokens("!=", { { Token::Operator, "!=" } }));
  ASSERT_FALSE(equalTokens("! =", { { Token::Operator, "!=" } }));

  const std::string code =
    "auto main() -> int\n"
    "{\n"
    "    bool isFirst = 100 < 200;\n"
    "    if (!isFirst || (100 != 200))\n"
    "    {\n"
    "        return 1;\n"
    "    }\n"
    "\n"
    "    return 0;\n"
    "}";
  EXPECT_EQ(countOfTokens(code, Token::Operator), 5);
  EXPECT_EQ(countOfTokens(code, Token::Punctuator), 14);
  ASSERT_TRUE(equalTokens(code,
    { { Token::Keyword, "auto" },
      { Token::Identifier, "main" },
      { Token::Punctuator, "(" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, "->" },
      { Token::Keyword, "int" },
      { Token::Punctuator, "{" },
      { Token::Keyword, "bool" },
      { Token::Identifier, "isFirst" },
      { Token::Operator, "=" },
      { Token::Literal, "100" },
      { Token::Operator, "<" },
      { Token::Literal, "200" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "if" },
      { Token::Punctuator, "(" },
      { Token::Operator, "!" },
      { Token::Identifier, "isFirst" },
      { Token::Operator, "||" },
      { Token::Punctuator, "(" },
      { Token::Literal, "100" },
      { Token::Operator, "!=" },
      { Token::Literal, "200" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, "{" },
      { Token::Keyword, "return" },
      { Token::Literal, "1" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "}" },
      { Token::Keyword, "return" },
      { Token::Literal, "0" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "}" } }));
}

/**
 * \brief          Проверка определения символов "+", "-", "*", "/", "%", "++", "--", "+=", "-=", "*=", "/=", "%="
 */
TEST_F(OpeartorsTest, ArithmeticOperators)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("+", { { Token::Operator, "+" } }));
  ASSERT_TRUE(equalTokens("-", { { Token::Operator, "-" } }));
  ASSERT_TRUE(equalTokens("*", { { Token::Operator, "*" } }));
  ASSERT_TRUE(equalTokens("/", { { Token::Operator, "/" } }));
  ASSERT_TRUE(equalTokens("%", { { Token::Operator, "%" } }));
  ASSERT_TRUE(equalTokens("++", { { Token::Operator, "++" } }));
  ASSERT_TRUE(equalTokens("--", { { Token::Operator, "--" } }));
  ASSERT_TRUE(equalTokens("+=", { { Token::Operator, "+=" } }));
  ASSERT_TRUE(equalTokens("-=", { { Token::Operator, "-=" } }));
  ASSERT_TRUE(equalTokens("*=", { { Token::Operator, "*=" } }));
  ASSERT_TRUE(equalTokens("/=", { { Token::Operator, "/=" } }));
  ASSERT_TRUE(equalTokens("%=", { { Token::Operator, "%=" } }));
  ASSERT_FALSE(equalTokens("+ +", { { Token::Operator, "++" } }));
  ASSERT_FALSE(equalTokens("- -", { { Token::Operator, "--" } }));
  ASSERT_FALSE(equalTokens("+ =", { { Token::Operator, "+=" } }));
  ASSERT_FALSE(equalTokens("- =", { { Token::Operator, "-=" } }));
  ASSERT_FALSE(equalTokens("* =", { { Token::Operator, "*=" } }));
  ASSERT_FALSE(equalTokens("/ =", { { Token::Operator, "/=" } }));
  ASSERT_FALSE(equalTokens("% =", { { Token::Operator, "%=" } }));

  const std::string code =
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
    "}";
  EXPECT_EQ(countOfTokens(code, Token::Operator), 21);
  EXPECT_EQ(countOfTokens(code, Token::Punctuator), 22);
  ASSERT_TRUE(equalTokens(code,
    { { Token::Keyword, "auto" },
      { Token::Identifier, "main" },
      { Token::Punctuator, "(" },
      { Token::Punctuator, ")" },
      { Token::Punctuator, "->" },
      { Token::Keyword, "int" },
      { Token::Punctuator, "{" },
      { Token::Keyword, "int" },
      { Token::Identifier, "foo" },
      { Token::Operator, "=" },
      { Token::Literal, "100" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "int" },
      { Token::Identifier, "bar" },
      { Token::Operator, "=" },
      { Token::Literal, "200" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "foo" },
      { Token::Operator, "=" },
      { Token::Identifier, "bar" },
      { Token::Operator, "+" },
      { Token::Literal, "10" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "=" },
      { Token::Identifier, "foo" },
      { Token::Operator, "-" },
      { Token::Literal, "20" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "foo" },
      { Token::Operator, "=" },
      { Token::Identifier, "bar" },
      { Token::Operator, "*" },
      { Token::Literal, "2" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "=" },
      { Token::Identifier, "foo" },
      { Token::Operator, "/" },
      { Token::Literal, "20" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "=" },
      { Token::Identifier, "foo" },
      { Token::Operator, "%" },
      { Token::Literal, "2" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "++" },
      { Token::Punctuator, ";" },
      { Token::Operator, "++" },
      { Token::Identifier, "bar" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "foo" },
      { Token::Operator, "--" },
      { Token::Punctuator, ";" },
      { Token::Operator, "--" },
      { Token::Identifier, "foo" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "foo" },
      { Token::Operator, "+=" },
      { Token::Literal, "10" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "-=" },
      { Token::Literal, "20" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "foo" },
      { Token::Operator, "*=" },
      { Token::Literal, "2" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "/=" },
      { Token::Literal, "20" },
      { Token::Punctuator, ";" },
      { Token::Identifier, "bar" },
      { Token::Operator, "%=" },
      { Token::Literal, "2" },
      { Token::Punctuator, ";" },
      { Token::Keyword, "return" },
      { Token::Literal, "0" },
      { Token::Punctuator, ";" },
      { Token::Punctuator, "}" } }));
}

/**
 * \brief          Проверка определения символов "&", "&=", "&&", "|", "|=", "||", "!", "!="
 */
TEST_F(OpeartorsTest, LogicalOperators)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("&", { { Token::Operator, "&" } }));
  ASSERT_TRUE(equalTokens("&=", { { Token::Operator, "&=" } }));
  ASSERT_TRUE(equalTokens("&&", { { Token::Operator, "&&" } }));
  ASSERT_TRUE(equalTokens("|", { { Token::Operator, "|" } }));
  ASSERT_TRUE(equalTokens("|=", { { Token::Operator, "|=" } }));
  ASSERT_TRUE(equalTokens("||", { { Token::Operator, "||" } }));
  ASSERT_TRUE(equalTokens("!", { { Token::Operator, "!" } }));
  ASSERT_TRUE(equalTokens("!=", { { Token::Operator, "!=" } }));

  ASSERT_FALSE(equalTokens("& =", { { Token::Operator, "&=" } }));
  ASSERT_FALSE(equalTokens("& &", { { Token::Operator, "&&" } }));
  ASSERT_FALSE(equalTokens("| =", { { Token::Operator, "|=" } }));
  ASSERT_FALSE(equalTokens("| |", { { Token::Operator, "||" } }));
  ASSERT_FALSE(equalTokens("! =", { { Token::Operator, "!=" } }));
}

/**
 * \brief          Проверка определения символов "&", "|", "^", "~", "<<", ">>"
 */
TEST_F(OpeartorsTest, BitwiseOperators)
{
  using origami::lex::Token;

  ASSERT_TRUE(equalTokens("&", { { Token::Operator, "&" } }));
  ASSERT_TRUE(equalTokens("|", { { Token::Operator, "|" } }));
  ASSERT_TRUE(equalTokens("^", { { Token::Operator, "^" } }));
  ASSERT_TRUE(equalTokens("~", { { Token::Operator, "~" } }));
  ASSERT_TRUE(equalTokens("<<", { { Token::Operator, "<<" } }));
  ASSERT_TRUE(equalTokens(">>", { { Token::Operator, ">>" } }));
  ASSERT_FALSE(equalTokens("< <", { { Token::Operator, "<<" } }));
  ASSERT_FALSE(equalTokens("> >", { { Token::Operator, ">>" } }));
}