#include "origami_lexical/conventions/tokens.hpp"

#include <algorithm>
#include <cctype>
#include <vector>

#include <gtest/gtest.h>

class LexicalConventions {
  public:
    explicit LexicalConventions(std::string t_code) : m_code { std::move(t_code) }
    {
    }

    std::vector<origami::lex::token::Token> getTokens();

  private:
    std::string m_code;
};

std::vector<origami::lex::token::Token> LexicalConventions::getTokens()
{
  // Буфер, куда будем сохранять значения лексического анализатора
  std::vector<origami::lex::token::Token> tokens;

  // Инициализируем стартовую позицию чтения исходного кода
  std::size_t current_symbol = 0;

  // Пока символы не закончатся, считываем их последовательно
  while (current_symbol != m_code.size()) {
    // Если символ относится к категории 'пробельный символ', игнорируем его
    if (std::isspace(m_code[current_symbol])) {
      ++current_symbol;
    // Если символ относится к категории 'буквенный символ', определяем полный его идентификатор
    } else if (std::isalpha(m_code[current_symbol])) {
      auto finished_isalnum = std::find_if_not(m_code.begin() + current_symbol, m_code.end(), [](auto t_ch) {
        return std::isalnum(t_ch, std::locale { "C" });
      });

      if (finished_isalnum != m_code.end()) {
        std::cout << m_code.substr(current_symbol, std::distance(m_code.begin(), finished_isalnum) - current_symbol) << std::endl;
        current_symbol = std::distance(m_code.begin(), finished_isalnum);
      }
    // Если символ относится к категории 'цифровой символ', определяем полный его идентификатор
    } else if (std::isdigit(m_code[current_symbol])) {
      auto finished_isalnum = std::find_if_not(m_code.begin() + current_symbol, m_code.end(), [](auto t_ch) {
        return (std::isalnum(t_ch, std::locale { "C" }) || (t_ch == '.') || (t_ch == '+') || (t_ch == '-'));
      });

      if (finished_isalnum != m_code.end()) {
        std::cout << m_code.substr(current_symbol, std::distance(m_code.begin(), finished_isalnum) - current_symbol) << std::endl;
        current_symbol = std::distance(m_code.begin(), finished_isalnum);
      }
    // Все символы, которые относятся к группе 'операторы и пунктуация', интерпретируем пока через 'switch'
    } else {
      switch (m_code[current_symbol]) {
        case '{' :
          std::cout << '{' << std::endl;
          ++current_symbol;
          break;
        case '}' :
          std::cout << '}' << std::endl;
          ++current_symbol;
          break;
        case '(' :
          std::cout << '(' << std::endl;
          ++current_symbol;
          break;
        case ')' :
          std::cout << ')' << std::endl;
          ++current_symbol;
          break;
        case ';' :
          std::cout << ';' << std::endl;
          ++current_symbol;
          break;
      }
    }
  }

  return tokens;
}

TEST(LexicalConventions, preprocessing_tokens)
{
  auto tokens = LexicalConventions("int main()\n{\n    return 0;\n}").getTokens();
  std::vector<origami::lex::token::Token> expect_tokens
  {
  origami::lex::token::Token::Keyword,
  origami::lex::token::Token::Identifier,
  origami::lex::token::Token::Punctuator,
  origami::lex::token::Token::Punctuator,
  origami::lex::token::Token::Punctuator,
  origami::lex::token::Token::Keyword,
  origami::lex::token::Token::Literal,
  origami::lex::token::Token::Punctuator,
  origami::lex::token::Token::Punctuator
  };

  ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin()));
}

int main(int t_argc, char** t_argv)
{
  ::testing::InitGoogleTest(&t_argc, t_argv);

  return RUN_ALL_TESTS();
}