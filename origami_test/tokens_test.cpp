#include "origami_lexical/conventions/tokens.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <vector>

#include <gtest/gtest.h>

/*
TODO: Заложить структуру, для работы с Unreal Engine и ROS, а так же с другими библиотеками, по необходимости.
http://cpp-reference.ru/patterns/structural-patterns/bridge/
class LexicalConventionsCpp {};
class LexicalConventionsRos : public LexicalConventionsCpp {};
class LexicalConventionsUnrealEngine : public LexicalConventionsCpp {};
*/

// TODO: Перенести в отдельный класс, файл
class LexicalConventions {
  public:
    explicit LexicalConventions(std::string t_code) : m_code { std::move(t_code) }
    {
      // TODO: Заполнить. Такой подход не очень хорош, так как при создании объекта требуется новое создание слов. Исправить.
      m_keywords.emplace("int", origami::lex::Keywords::Int);
      m_keywords.emplace("return", origami::lex::Keywords::Return);
    }

    std::vector<std::pair<origami::lex::Token, std::string>> getTokens();

  private:
    const std::string m_code; ///< Исходный код программы.

    std::map<std::string, origami::lex::Keywords> m_keywords;
};

// TODO: Представить значение Lexeme как значения enum class, а не std::string. Продумать, как это сделать. И надо ли?
// Это будет понятно только в момент синтаксического и семантического анализа
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

      // Формируем слово
      auto word = m_code.substr(current_symbol, std::distance(m_code.begin(), not_isalnum) - current_symbol);

      // Находим ключевое слово, которое описано в стандарте С++ 5.11 таблица 5
      const auto keyword = m_keywords.find(word);

      // Если нашли ключевое слово, идентифицируем его как ключевое слово
      if (keyword != m_keywords.end()) {
        tokens.emplace_back(origami::lex::Token::Keyword, keyword->first);
      } else { // Иначе, это именование
        tokens.emplace_back(origami::lex::Token::Identifier, std::move(word));
      }

      // Инициализируем последней позицией символа, на котором заканчивается найденное слово
      current_symbol = std::distance(m_code.begin(), not_isalnum);

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
      // Все символы, которые относятся к группе 'пунктуация'
    } else if (std::ispunct(static_cast<unsigned char>(m_code[current_symbol]))) {
      switch (m_code[current_symbol]) {
        case '#' : {
          /* Пока добавляем поддержку только указанных препроцессоров
           *  i - #if #ifdef #ifndef #include
           *  e - #elif #else #endif #error
           *  d - #define
           *  l - #line
           *  u - #undef #using
           * */

          // Находим первый символ, который не относится к пробелу. Так как пробелы можно ставить, все остальное - ошибка
          // Примеры '#      include' - можно, '#   \n   include' - нельзя
          const auto start_preprocessor = m_code.find_first_not_of(' ', current_symbol + 1);

          switch (m_code[start_preprocessor]) {
            case 'i' : { // #if #ifdef #ifndef #include
              // Обработка предпроцессора '#include'
              if (auto find_include = m_code.find("include", start_preprocessor); find_include == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#include'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#include" });

                // Аналогично поиску позиции препроцессора, поиск начала header-file
                const auto left_mark = m_code.find_first_not_of(' ', start_preprocessor + 7); // 7 - length("include")

                // Смотрим, что найденный символ должен иметь значение '<' или '"', так как подключение заголовочных файлов начинается
                // с данных символов, и оговорено стандартом С++ в пункте 5.8
                if ((left_mark != std::string::npos) && (m_code[left_mark] != '<') && (m_code[left_mark] != '\"')) {
                  current_symbol = left_mark;
                  break;
                }

                // Находим закрывающий оператор, '>' или '"'. Тут не проверяются случаи, когда #include <header-name" или
                // #include "header-name>. Так как эта часть проверки осуществляется синтаксическим анализатором
                const auto right_mark = m_code.find_first_of(">\"", left_mark + 1);
                tokens.emplace_back(origami::lex::Token::Identifier, m_code.substr(left_mark, right_mark - left_mark + 1));
                current_symbol = right_mark + 1;
              } else if (auto find_ifdef = m_code.find("ifdef", start_preprocessor); find_ifdef == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#ifdef'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#ifdef" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 5;
              } else if (auto find_ifndef = m_code.find("ifndef", start_preprocessor); find_ifndef == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#ifndef'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#ifndef" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 6;
              }

              break;
            }
            case 'd' : {
              if (auto find_define = m_code.find("define", start_preprocessor); find_define == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#define'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#define" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 6;
              }

              break;
            }
            case 'e' : {
              if (auto find_endif = m_code.find("endif", start_preprocessor); find_endif == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#endif'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#endif" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 5;
              }

              break;
            }
            default: {
              tokens.emplace_back(origami::lex::Token::Punctuator, std::string { m_code[current_symbol] });
              ++current_symbol;
            }
          }

          break;
        }
        case '+' : {
          tokens.emplace_back(origami::lex::Token::Operator, std::string { m_code[current_symbol] });
          ++current_symbol;
          break;
        }
        default: {
          tokens.emplace_back(origami::lex::Token::Punctuator, std::string { m_code[current_symbol] });
          ++current_symbol;
        }
      }
    }
  }

  return tokens;
}

TEST(LexicalConventions, Main)
{
  const auto tokens = LexicalConventions("int main()\n{\n    return 0;\n}").getTokens();
  const std::array<std::pair<origami::lex::Token, std::string>, 9> expect_tokens {
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

  ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
    return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
  }));
}

TEST(LexicalConventionsPreprocessor, PreprocessorInclude)
{
  { // Проверка, на определение простого символа '#'
    const auto tokens = LexicalConventions("#").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 1> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = LexicalConventions("#     include      <memory>      ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 2> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "<memory>" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате "header-name"
    const auto tokens = LexicalConventions("#     include      \"memory\"      ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 2> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "\"memory\"" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = LexicalConventions("#  +   include      \"memory\"      ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 6> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Identifier, "include" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = LexicalConventions("#     include    +    \"memory\"      ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 6> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = LexicalConventions("#     include   int    \"memory\"      ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 6> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение нескольких заголовочных файлов
    const auto tokens = LexicalConventions("#     include      <memory>\n  #     include      \"algorithm\"    ").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 4> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "<memory>" },
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "\"algorithm\"" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Простая программа с подключением двух заголовочных файлов
    const auto tokens = LexicalConventions("#include <memory>\n#     include      \"origami_lexical/conventions/tokens.hpp\"\n    "
                                           "int main()\n{\n\treturn 0;\n}").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 13> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "<memory>" },
        { origami::lex::Token::Keyword, "#include" },
        { origami::lex::Token::Identifier, "\"origami_lexical/conventions/tokens.hpp\"" },
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

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST(LexicalConventionsPreprocessor, PreprocessorIfdef)
{
  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = LexicalConventions("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXY").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 2> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXY" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = LexicalConventions("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz   \n"
                                           "int main()\n{\n\treturn 0;\n}").getTokens();
    const std::array<std::pair<origami::lex::Token, std::string>, 11> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
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

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST(LexicalConventionsPreprocessor, PreprocessorAll)
{
  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = LexicalConventions("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz   \n"
                                           "#       define  NUM_SIZE  0.0\n"
                                           "#       endif \n"
                                           "#   ifndef ___0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                                           "#define MCVC_USE\n"
                                           "#       endif \n"
                                           "int main()\n{\n\treturn 0;\n}").getTokens();
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
        { origami::lex::Token::Keyword, "#define" },
        { origami::lex::Token::Identifier, "NUM_SIZE" },
        { origami::lex::Token::Literal, "0.0" },
        { origami::lex::Token::Keyword, "#endif" },
        { origami::lex::Token::Keyword, "#ifndef" },
        { origami::lex::Token::Identifier, "___0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
        { origami::lex::Token::Keyword, "#define" },
        { origami::lex::Token::Identifier, "MCVC_USE" },
        { origami::lex::Token::Keyword, "#endif" },
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

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

int main(int t_argc, char** t_argv)
{
  ::testing::InitGoogleTest(&t_argc, t_argv);

  return RUN_ALL_TESTS();
}