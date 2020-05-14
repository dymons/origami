#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <algorithm>
#include <cctype>
#include <locale>

#ifdef NDEBUG
#include <cassert>
#include <iostream>

#endif

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
        tokens.emplace_back(origami::lex::Token::Keyword, *keyword);
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
          if (const auto start_preprocessor = t_code.find_first_not_of(' ', current_symbol + 1); start_preprocessor != std::string::npos) {
            std::string::size_type exist_keyword = std::string::npos;
            for (const auto& preprocessor_keyword : m_preprocessorKeywords) {
              if (exist_keyword = t_code.find(preprocessor_keyword, start_preprocessor); exist_keyword == start_preprocessor) {
                const auto second_symbol = std::next(t_code.begin(), start_preprocessor + preprocessor_keyword.size());
                if ((second_symbol != t_code.end()) && (!std::isspace(*second_symbol))) {
                  continue;
                }

                tokens.emplace_back(origami::lex::Token::KeywordPreprocessor, "#" + preprocessor_keyword);
                current_symbol = start_preprocessor + preprocessor_keyword.size();
                break;
              }
            }

            if (exist_keyword == start_preprocessor) {
              break;
            }
          }

          tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
          ++current_symbol;

          break;
        }
        case '>' : {
          // Получаем все операторы, которые можно скомбинировать с символом '>'. А именно ">", ">=", ">>", ">>="
          const auto operators = m_operators.at(t_code[current_symbol]);

          std::string::size_type operator_boundary = current_symbol; // Граница последнего корректного символа
          std::string operator_build = t_code.substr(operator_boundary, 1); // Собираем символ как мозаику, последовательно

          ++operator_boundary; // Берем следующий символ
          while (operator_boundary != t_code.size()) {
            // Строим символ опуская пробелы, так как можно делать так std::vector<std::set<int> >, '> >'
            if (const auto not_whitespace = t_code.find_first_not_of(' ', operator_boundary); not_whitespace != std::string::npos) {
              if (operators.find(operator_build + t_code[not_whitespace]) != operators.end()) {
                operator_boundary = not_whitespace + 1;
                operator_build += t_code[not_whitespace];
              } else {
                // Если не удалось найти комбинацию 'operator_build' + 'next symbol', среди известных комбинаций,
                // завершаем работу
                break;
              }
            } else {
              // Позиция символа не было определенно, как пример '<  '
              break;
            }
          };

          tokens.emplace_back(origami::lex::Token::Punctuator, std::move(operator_build));
          current_symbol = operator_boundary;

          break;
        }
        case '<' : {
          const auto operators = m_operators.at(t_code[current_symbol]);

          std::string::size_type operator_boundary = current_symbol;
          std::string operator_build = t_code.substr(operator_boundary, 1);

          ++operator_boundary;
          while (operator_boundary != t_code.size()) {
            if (operators.find(operator_build + t_code[operator_boundary]) != operators.end()) {
              operator_build += t_code[operator_boundary];
              ++operator_boundary;
            } else {
              break;
            }
          };

          tokens.emplace_back(origami::lex::Token::Punctuator, std::move(operator_build));
          current_symbol = operator_boundary;

          break;
        }
        case '+' : {
          // TODO: Определиться, какие операторы операции, а какие пунктуации
          tokens.emplace_back(origami::lex::Token::Operator, std::string { t_code[current_symbol] });
          ++current_symbol;
          break;
        }
        case '"' : {
          if (const auto last_mark = t_code.find_first_of('"', current_symbol + 1); last_mark != std::string::npos) {
            tokens.emplace_back(origami::lex::Token::Literal, t_code.substr(current_symbol, last_mark - current_symbol + 1));
            current_symbol = last_mark + 1;
          } else {
            tokens.emplace_back(origami::lex::Token::Punctuator, std::string { t_code[current_symbol] });
            ++current_symbol;
          }

          break;
        }
        default: {
          const auto operators = m_operators.at(t_code[current_symbol]);

          std::string::size_type operator_boundary = current_symbol;
          std::string operator_build = t_code.substr(operator_boundary, 1);

          ++operator_boundary;
          while (operator_boundary != t_code.size()) {
            if (operators.find(operator_build + t_code[operator_boundary]) != operators.end()) {
              operator_build += t_code[operator_boundary];
              ++operator_boundary;
            } else {
              break;
            }
          };

          tokens.emplace_back(origami::lex::Token::Punctuator, std::move(operator_build));
          current_symbol = operator_boundary;
        }
      }
    }

#ifdef NDEBUG
    else {
      assert(false);
    }
#endif

  }

  return tokens;
}
} // namespace origami::lex