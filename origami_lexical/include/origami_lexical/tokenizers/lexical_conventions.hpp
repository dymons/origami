/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Лексический анализатор, разбитвает исходный код программы на последовательность лексем
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_CONVENTIONS_HPP
#define ORIGAMI_LEXICAL_CONVENTIONS_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/conventions/tokens.hpp"
#include "origami_lexical/symbol_table/symbol_table_cpp.hpp"
#include "origami_lexical/symbol_table/symbol_table_python.hpp"

#ifdef ORIGAMI_DEBUG
#include <cassert>
#include <iostream>
#endif

#include <deque>
#include <algorithm>
#include <cctype>
#include <locale>

namespace origami::lex {

/**
 * \brief           Аналитического разбор входной последовательности символов на распознанные группы — лексемы — с целью получения
 *                  на выходе идентифицированных последовательностей, называемых «токенами». Аналитический разбор основан на анализе
 *                  стандарта С++17.
 */
template<typename T> class LexicalConventions
{
public:
  LexicalConventions() : m_table(std::make_shared<T>()) {}

  /**
   * \brief          Формирует последовательность token'ов на основе исходного кода программы
   *
   * \param[in]      t_code - исходный код программы
   *
   * \return         Последовательность token'ов
   */
  [[nodiscard]] std::deque<std::pair<origami::lex::Token, Lexeme>> getTokens(const std::string& t_code)
  {
    // Буфер, куда будем сохранять значения лексического анализатора
    std::deque<std::pair<origami::lex::Token, Lexeme>> tokens;

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
        const auto keyword = m_table->keywords().find(word);

        // Если нашли ключевое слово, идентифицируем его как ключевое слово
        if (keyword != m_table->keywords().end()) {
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
        if (const auto punctuation = m_table->punctuation().find(t_code[current_symbol]); punctuation != m_table->punctuation().end()) {
          // Находим максимульную длину возможной комбинации для текущего символа
          const auto max_combination = std::max_element(punctuation->second.begin(), punctuation->second.end());

          // Приводим максимальную длину к актуальному значению, для того чтобы не выйти за границы массива
          auto max_size = std::clamp<decltype(current_symbol)>(max_combination->size(), 0, t_code.size() - current_symbol);

          // Производим поиск комбинации с самой длинной возможной конструктуции, до тех пор пока не найдем, либо не выйдем из цикла
          do {
            // Если нашли нужную комбинацию
            if (auto punct = punctuation->second.find(t_code.substr(current_symbol, max_size)); punct != punctuation->second.end()) {
              tokens.emplace_back(origami::lex::Token::Punctuator, *punct);
              current_symbol += max_size;
              break;
            }
          } while (--max_size != 0);
        }

        if (const auto operators = m_table->operators().find(t_code[current_symbol]); operators != m_table->operators().end()) {
          // Находим максимульную длину возможной комбинации для текущего символа
          const auto max_combination = std::max_element(operators->second.begin(), operators->second.end());

          // Приводим максимальную длину к актуальному значению, для того чтобы не выйти за границы массива
          auto max_size = std::clamp<decltype(current_symbol)>(max_combination->size(), 0, t_code.size() - current_symbol);

          // Производим поиск комбинации с самой длинной возможной конструктуции, до тех пор пока не найдем, либо не выйдем из цикла
          do {
            // Если нашли нужную комбинацию
            if (auto punct = operators->second.find(t_code.substr(current_symbol, max_size)); punct != operators->second.end()) {
              tokens.emplace_back(origami::lex::Token::Operator, *punct);
              current_symbol += max_size;
              break;
            }
          } while (--max_size != 0);
        }

        switch (t_code[current_symbol]) {
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
        }
      }
    }

    return tokens;
  }

private:
  SymbolTableImplPtr m_table;///< Символьная таблица, содержащая информацию о синтаксисе языка программирования
};

template class LexicalConventions<SymbolTableCpp>;
template class LexicalConventions<SymbolTablePython>;

}// namespace origami::lex

#endif// ORIGAMI_LEXICAL_CONVENTIONS_HPP