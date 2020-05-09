#include "origami_lexical/conventions/tokens.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <vector>

#include <gtest/gtest.h>

class LexicalConventions {
  public:
    explicit LexicalConventions(std::string t_code) : m_code { std::move(t_code) }
    {
      m_keywords.emplace("int", origami::lex::Keywords::Int);
      m_keywords.emplace("return", origami::lex::Keywords::Return);
    }

    std::vector<std::pair<origami::lex::Token, std::string>> getTokens();

  private:
    const std::string m_code;

    std::map<std::string, origami::lex::Keywords> m_keywords;
};

std::vector<std::pair<origami::lex::Token, std::string>> LexicalConventions::getTokens()
{
  // Буфер, куда будем сохранять значения лексического анализатора
  std::vector<std::pair<origami::lex::Token, std::string>> tokens;

  // Инициализируем стартовую позицию чтения исходного кода
  std::size_t current_symbol = 0;

  // Пока символы не закончатся, считываем их последовательно
  while (current_symbol != m_code.size()) {
    // Если символ относится к категории 'пробельный символ', игнорируем его
    if (std::isspace(m_code[current_symbol])) {
      ++current_symbol;
      // Если символ относится к категории 'буквенный символ', определяем полный его идентификатор
      // '_' означает, что именование функций / переменных / классов и т.д. может начинаться с нижнего подчеркивания
    } else if (std::isalpha(m_code[current_symbol]) || (m_code[current_symbol] == '_')) {
      auto not_isalnum = std::find_if_not(m_code.begin() + current_symbol, m_code.end(), [](auto t_ch) {
        return std::isalnum(t_ch, std::locale { "C" }) || (t_ch == '_'); // Для примера, static_cast имеет '_', поэтому нужно учитывать
      });

      if (not_isalnum != m_code.end()) {
        auto word = m_code.substr(current_symbol, std::distance(m_code.begin(), not_isalnum) - current_symbol);
        // Находим ключевое слово, которое описано в стандарте С++ 5.11 таблица 5
        auto keyword = m_keywords.find(word);

        // Если нашли ключевое слово, идентифицируем его как ключевое слово
        if (keyword != m_keywords.end()) {
          tokens.emplace_back(origami::lex::Token::Keyword, keyword->first);
        } else { // Иначе, это именование
          tokens.emplace_back(origami::lex::Token::Identifier, std::move(word));
        }

        // Инициализируем последней позицией символа, на котором заканчивается найденное слово
        current_symbol = std::distance(m_code.begin(), not_isalnum);
      }
      // Если символ относится к категории 'цифровой символ', определяем полный его идентификатор
    } else if (std::isdigit(m_code[current_symbol], std::locale { "C" })) {
      auto not_isdigit = std::find_if_not(m_code.begin() + current_symbol, m_code.end(), [](auto t_ch) {
        return (std::isdigit(t_ch, std::locale { "C" }) || (t_ch == '.') || (t_ch == '+') || (t_ch == '-'));
      });

      if (not_isdigit != m_code.end()) {
        auto digital = m_code.substr(current_symbol, std::distance(m_code.begin(), not_isdigit) - current_symbol);
        tokens.emplace_back(origami::lex::Token::Literal, std::move(digital));
        current_symbol = std::distance(m_code.begin(), not_isdigit);
      }
      // Все символы, которые относятся к группе 'операторы и пунктуация', интерпретируем пока через 'switch'
    } else {
      switch (m_code[current_symbol]) {
        case '{' :
          tokens.emplace_back(origami::lex::Token::Punctuator, "{");
          ++current_symbol;
          break;
        case '}' :
          tokens.emplace_back(origami::lex::Token::Punctuator, "}");
          ++current_symbol;
          break;
        case '(' :
          tokens.emplace_back(origami::lex::Token::Punctuator, "(");
          ++current_symbol;
          break;
        case ')' :
          tokens.emplace_back(origami::lex::Token::Punctuator, ")");
          ++current_symbol;
          break;
        case ';' :
          tokens.emplace_back(origami::lex::Token::Punctuator, ";");
          ++current_symbol;
          break;
      }
    }
  }

  return tokens;
}

TEST(LexicalConventions, preprocessing_tokens)
{
  const auto tokens = LexicalConventions("int main()\n{\n    return 0;\n}").getTokens();
  std::array<std::pair<origami::lex::Token, std::string>, 9> expect_tokens {
    {
      { origami::lex::Token::Keyword, "int" },
      { origami::lex::Token::Identifier, "main" },
      { origami::lex::Token::Punctuator, "(" },
      { origami::lex::Token::Punctuator, ")" },
      { origami::lex::Token::Punctuator, "{" },
      { origami::lex::Token::Keyword, "return" },
      { origami::lex::Token::Literal, "0" },
      { origami::lex::Token::Punctuator, ";" },
      { origami::lex::Token::Punctuator, "}" }
    }
  };

  ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](auto& t_lhs, auto& t_rhs) {
    return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
  }));
}

int main(int t_argc, char** t_argv)
{
  ::testing::InitGoogleTest(&t_argc, t_argv);

  return RUN_ALL_TESTS();
}