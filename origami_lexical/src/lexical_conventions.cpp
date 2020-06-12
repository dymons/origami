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
  std::string::size_type current_symbol = 0;

  // Пока символы не закончатся, считываем их последовательно
  while (current_symbol != t_code.size()) {
    // Если символ относится к категории 'пробельный символ', игнорируем его
    if (std::isspace(t_code[current_symbol]) != 0) {
      ++current_symbol;
      // Если символ относится к категории 'буквенный символ', определяем полный его идентификатор
      // '_' означает, что именование функций / переменных / классов и т.д. может начинаться с нижнего подчеркивания
    } else if ((std::isalpha(t_code[current_symbol]) != 0) || (t_code[current_symbol] == '_')) {
      auto not_isalnum = std::find_if_not(t_code.begin() + current_symbol, t_code.end(), [](auto t_ch) {
        return std::isalnum(t_ch, std::locale{ "C" }) || (t_ch == '_');// Для примера, static_cast имеет '_', поэтому нужно учитывать
      });

      // Формируем слово
      auto word = t_code.substr(current_symbol, std::distance(t_code.begin(), not_isalnum) - current_symbol);

      // Находим ключевое слово, которое описано в стандарте С++ 5.11 таблица 5
      const auto keyword = m_keywords.find(word);

      // Если нашли ключевое слово, идентифицируем его как ключевое слово
      if (keyword != m_keywords.end()) {
        tokens.emplace_back(origami::lex::Token::Keyword, *keyword);
      } else {// Иначе, это именование
        tokens.emplace_back(origami::lex::Token::Identifier, std::move(word));
      }

      // Инициализируем последней позицией символа, на котором заканчивается найденное слово
      current_symbol = static_cast<decltype(current_symbol)>(std::distance(t_code.begin(), not_isalnum));

      // Если символ относится к категории 'цифровой символ', определяем полный его идентификатор
    } else if (std::isdigit(t_code[current_symbol], std::locale{ "C" })) {
      auto not_isdigit = std::find_if_not(t_code.begin() + current_symbol, t_code.end(), [](auto t_ch) {
        return (std::isdigit(t_ch, std::locale{ "C" }) || (t_ch == '.') || (t_ch == '+') || (t_ch == '-'));
      });

      if (not_isdigit != t_code.end()) {
        auto digital = t_code.substr(current_symbol, std::distance(t_code.begin(), not_isdigit) - current_symbol);
        tokens.emplace_back(origami::lex::Token::Literal, std::move(digital));
        current_symbol = static_cast<decltype(current_symbol)>(std::distance(t_code.begin(), not_isdigit));
      }
    } else {
      if (const auto punctuation = m_punctuation.find(t_code[current_symbol]); punctuation != m_punctuation.end()) {
        std::string::size_type punctuator_boundary = current_symbol;
        std::string punctuator_build = t_code.substr(punctuator_boundary, 1);

        ++punctuator_boundary;
        while (punctuator_boundary != t_code.size()) {
          if (punctuation->second.find(punctuator_build + t_code[punctuator_boundary]) != punctuation->second.end()) {
            punctuator_build += t_code[punctuator_boundary];
            ++punctuator_boundary;
          } else {
            break;
          }
        }

        if (punctuation->second.find(punctuator_build) != punctuation->second.end()) {
          tokens.emplace_back(origami::lex::Token::Punctuator, std::move(punctuator_build));
          current_symbol = punctuator_boundary;
          continue;
        }
      }

      if (const auto operators = m_operators.find(t_code[current_symbol]); operators != m_operators.end()) {
        std::string::size_type operator_boundary = current_symbol;
        std::string operator_build = t_code.substr(operator_boundary, 1);

        ++operator_boundary;
        while (operator_boundary != t_code.size()) {
          if (operators->second.find(operator_build + t_code[operator_boundary]) != operators->second.end()) {
            operator_build += t_code[operator_boundary];
            ++operator_boundary;
          } else {
            break;
          }
        }

        if (operators->second.find(operator_build) != operators->second.end()) {
          tokens.emplace_back(origami::lex::Token::Operator, std::move(operator_build));
          current_symbol = operator_boundary;
          continue;
        }
      }

      switch (t_code[current_symbol]) {
      case '#': {
        if (const auto start_preprocessor = t_code.find_first_not_of(' ', current_symbol + 1); start_preprocessor != std::string::npos) {
          std::string::size_type exist_keyword = std::string::npos;
          for (const auto& preprocessor_keyword : m_preprocessor_keywords) {
            if (exist_keyword = t_code.find(preprocessor_keyword, start_preprocessor); exist_keyword == start_preprocessor) {
              const auto second_symbol = std::next(t_code.begin(), start_preprocessor + preprocessor_keyword.size());
              if ((second_symbol != t_code.end()) && (std::isspace(*second_symbol) == 0)) { continue; }

              tokens.emplace_back(origami::lex::Token::KeywordPreprocessor, "#" + preprocessor_keyword);
              current_symbol = start_preprocessor + preprocessor_keyword.size();

              // Для предпроцессора include необходимо определить именование header файла в формате <h-char-sequence>
              if (preprocessor_keyword == "include") {
                // Находим первый символ, который не относится к пробелу
                if (const auto not_whitespace = t_code.find_first_not_of(' ', current_symbol); not_whitespace != std::string::npos) {
                  // Если это угловая скобка, значит мы на верном пути
                  if (t_code[not_whitespace] == '<') {
                    // Находим закрывающий символ '>'
                    if (const auto right_than_sign = t_code.find_first_of('>', not_whitespace); right_than_sign != std::string::npos) {
                      using origami::lex::Token;
                      tokens.emplace_back(Token::Literal, t_code.substr(not_whitespace, right_than_sign - not_whitespace + 1));
                      current_symbol = right_than_sign + 1;
                    }
                  }
                }
              }

              break;
            }
          }

          if (exist_keyword == start_preprocessor) { break; }
        }

        tokens.emplace_back(origami::lex::Token::Punctuator, std::string{ t_code[current_symbol] });
        ++current_symbol;

        break;
      }
      case '"': {
        if (const auto last_mark = t_code.find_first_of('"', current_symbol + 1); last_mark != std::string::npos) {
          tokens.emplace_back(origami::lex::Token::Literal, t_code.substr(current_symbol, last_mark - current_symbol + 1));
          current_symbol = last_mark + 1;
        } else {
          tokens.emplace_back(origami::lex::Token::Punctuator, std::string{ t_code[current_symbol] });
          ++current_symbol;
        }

        break;
      }
      default: {
#ifdef NDEBUG
        assert(false);
#endif
      }
      }
    }
  }

  return tokens;
}
}// namespace origami::lex