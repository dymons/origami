#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <algorithm>
#include <cctype>
#include <locale>

namespace origami::lex {
std::deque<std::pair<origami::lex::Token, std::string>> LexicalConventions::getTokens(const std::string& t_code)
{
  // Буфер, куда будем сохранять значения лексического анализатора
  std::deque<std::pair<origami::lex::Token, std::string>> tokens;

  // Инициализируем стартовую позицию чтения исходного кода
  std::size_t current_symbol = 0;

  // Пока символы не закончатся, считываем их последовательно
  while (current_symbol != t_code.size()) {
    // Если символ относится к категории 'пробельный символ', игнорируем его
    if (std::isspace(t_code[current_symbol])) {
      ++current_symbol;
      // Если символ относится к категории 'буквенный символ', определяем полный его идентификатор
      // '_' означает, что именование функций / переменных / классов и т.д. может начинаться с нижнего подчеркивания
    } else if (std::isalpha(t_code[current_symbol]) || (t_code[current_symbol] == '_')) {
      auto not_isalnum = std::find_if_not(t_code.begin() + current_symbol, t_code.end(), [](auto t_ch) {
        return std::isalnum(t_ch, std::locale { "C" }) || (t_ch == '_'); // Для примера, static_cast имеет '_', поэтому нужно учитывать
      });

      // Формируем слово
      auto word = t_code.substr(current_symbol, std::distance(t_code.begin(), not_isalnum) - current_symbol);

      // Находим ключевое слово, которое описано в стандарте С++ 5.11 таблица 5
      const auto keyword = m_keywords.find(word);

      // Если нашли ключевое слово, идентифицируем его как ключевое слово
      if (keyword != m_keywords.end()) {
        tokens.emplace_back(origami::lex::Token::Keyword, keyword->first);
      } else { // Иначе, это именование
        tokens.emplace_back(origami::lex::Token::Identifier, std::move(word));
      }

      // Инициализируем последней позицией символа, на котором заканчивается найденное слово
      current_symbol = std::distance(t_code.begin(), not_isalnum);

      // Если символ относится к категории 'цифровой символ', определяем полный его идентификатор
    } else if (std::isdigit(t_code[current_symbol], std::locale { "C" })) {
      auto not_isdigit = std::find_if_not(t_code.begin() + current_symbol, t_code.end(), [](auto t_ch) {
        return (std::isdigit(t_ch, std::locale { "C" }) || (t_ch == '.') || (t_ch == '+') || (t_ch == '-'));
      });

      if (not_isdigit != t_code.end()) {
        auto digital = t_code.substr(current_symbol, std::distance(t_code.begin(), not_isdigit) - current_symbol);
        tokens.emplace_back(origami::lex::Token::Literal, std::move(digital));
        current_symbol = std::distance(t_code.begin(), not_isdigit);
      }
      // Все символы, которые относятся к группе 'пунктуация'
    } else if (std::ispunct(static_cast<unsigned char>(t_code[current_symbol]))) {
      switch (t_code[current_symbol]) {
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
          const auto start_preprocessor = t_code.find_first_not_of(' ', current_symbol + 1);

          switch (t_code[start_preprocessor]) {
            case 'i' : { // #if #ifdef #ifndef #include
              // Обработка предпроцессора '#include'
              if (auto find_include = t_code.find("include", start_preprocessor); find_include == start_preprocessor) {
                const auto second_symbol = std::next(t_code.begin(), start_preprocessor + 7);
                if((second_symbol != t_code.end()) && (!std::isspace(*second_symbol))) {
                  tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
                  ++current_symbol;
                  break;
                }

                // Добавляем в tokens ключевое слово '#include'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#include" });

                if ((start_preprocessor + 7) == t_code.size()) {
                  current_symbol = start_preprocessor + 7;
                  break;
                }

                // Аналогично поиску позиции препроцессора, поиск начала header-file
                const auto left_mark = t_code.find_first_not_of(' ', start_preprocessor + 7); // 7 - length("include")

                // Смотрим, что найденный символ должен иметь значение '<' или '"', так как подключение заголовочных файлов начинается
                // с данных символов, и оговорено стандартом С++ в пункте 5.8
                if ((left_mark != std::string::npos) && (t_code[left_mark] != '<') && (t_code[left_mark] != '\"')) {
                  current_symbol = left_mark;
                  break;
                } else if (left_mark == std::string::npos) {
                  current_symbol = start_preprocessor + 7;
                  break;
                }

                // Находим закрывающий оператор, '>' или '"'. Тут не проверяются случаи, когда #include <header-name" или
                // #include "header-name>. Так как эта часть проверки осуществляется синтаксическим анализатором
                if (const auto right_mark = t_code.find_first_of(">\"", left_mark + 1); right_mark != std::string::npos) {
                  tokens.emplace_back(origami::lex::Token::Identifier, t_code.substr(left_mark, right_mark - left_mark + 1));
                  current_symbol = right_mark + 1;
                } else {
                  current_symbol = start_preprocessor + 7;
                }
              } else if (auto find_ifdef = t_code.find("ifdef", start_preprocessor); find_ifdef == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#ifdef'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#ifdef" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 5;
              } else if (auto find_ifndef = t_code.find("ifndef", start_preprocessor); find_ifndef == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#ifndef'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#ifndef" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 6;
              } else {
                tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
                ++current_symbol;
              }

              break;
            }
            case 'd' : {
              if (auto find_define = t_code.find("define", start_preprocessor); find_define == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#define'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#define" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 6;
              }

              break;
            }
            case 'e' : {
              if (auto find_endif = t_code.find("endif", start_preprocessor); find_endif == start_preprocessor) {
                // Добавляем в tokens ключевое слово '#endif'
                tokens.emplace_back(origami::lex::Token::Keyword, std::string { "#endif" });

                // Далее идет этап определения идентификатора
                current_symbol = start_preprocessor + 5;
              }

              break;
            }
            default: {
              tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
              ++current_symbol;
            }
          }

          break;
        }
        case '+' : {
          tokens.emplace_back(origami::lex::Token::Operator, std::string { t_code[current_symbol] });
          ++current_symbol;
          break;
        }
        default: {
          tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
          ++current_symbol;
        }
      }
    }
  }

  return tokens;
}
} // namespace origami::lex