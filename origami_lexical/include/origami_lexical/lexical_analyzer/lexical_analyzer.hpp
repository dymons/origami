/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Лексический анализатор, разбитвает исходный код программы на последовательность лексем
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_ANALYZER_HPP
#define ORIGAMI_LEXICAL_ANALYZER_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/conventions/tokens.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_cpp.hpp"
#include "origami_lexical/lexical_conventions/lexical_convention_python.hpp"
#include "origami_lexical/conventions/exeptions.hpp"

#include <algorithm>
#include <cctype>
#include <locale>
#include <type_traits>

#include <fmt/core.h>

namespace origami::lex {

template<typename T>
concept LexicalConvention = std::is_base_of<LexicalConventionImpl, T>::value;

/**
 * \brief           Аналитического разбор входной последовательности символов на распознанные группы — лексемы — с целью получения
 *                  на выходе идентифицированных последовательностей, называемых «токенами». Аналитический разбор основан на анализе
 *                  стандарта С++17.
 */
template<LexicalConvention T>
class LexicalAnalyzer
{
public:
  /**
   * \brief     Констуктор по умолчанию. Инициализация лексического соглашения.
   */
  LexicalAnalyzer() : m_convention(std::make_shared<T>()), m_code{}, m_current_symbol{ 0 }
  {}

  /**
   * \brief     Пользовательский конструктор. Инициализация лексического соглашения и обрабатываемого исходного кода программы
   *
   * \param[in] t_code - исходный код программы
   */
  explicit LexicalAnalyzer(std::string t_code) : m_convention(std::make_shared<T>()), m_code(std::move(t_code)), m_current_symbol{ 0 }
  {}

  /**
   * \brief     Последовательное получение токенов по запросам от внешней сущности
   *
   * \details   В зависимости от лексического соглашения, разбивает программу на 5 категорий токена. В случае, окончания работы лексического
   *            анализа, возвращается значение Eof, которое говорит о достижении конца файла.
   *
   * \return    Возвращает токен из заданного исходного кода программы
   */
  [[nodiscard]] std::pair<origami::lex::Token, Lexeme> getToken()
  {
    // Пока символы не закончатся, считываем их последовательно
    while (m_current_symbol != m_code.size()) {
      // Если символ относится к категории 'пробельный символ', игнорируем его
      if (std::isspace(m_code[m_current_symbol]) != 0) {
        ++m_current_symbol;
        // Если символ относится к категории 'буквенный символ', определяем полный его идентификатор
        // '_' означает, что именование функций / переменных / классов и т.д. может начинаться с нижнего подчеркивания
      } else if ((std::isalpha(m_code[m_current_symbol]) != 0) || (m_code[m_current_symbol] == '_')) {
        const auto current_pose_symbol = static_cast<std::string::difference_type>(m_current_symbol);
        const auto not_isalnum = std::find_if_not(std::next(m_code.begin(), current_pose_symbol), m_code.end(), [](const auto t_ch) {
          return std::isalnum(t_ch, std::locale{ "C" }) || (t_ch == '_');// Для примера, static_cast имеет '_', поэтому нужно учитывать
        });

        // Формируем слово
        const auto endOfWord = std::distance(m_code.begin(), not_isalnum);
        auto word = m_code.substr(m_current_symbol, static_cast<std::string::size_type>(endOfWord) - m_current_symbol);

        // Находим ключевое слово, которое описано в стандарте С++ 5.11 таблица 5
        const auto keyword = m_convention->keywords().find(word);

        // Инициализируем последней позицией символа, на котором заканчивается найденное слово
        m_current_symbol = static_cast<decltype(m_current_symbol)>(std::distance(m_code.begin(), not_isalnum));

        // Если нашли ключевое слово, идентифицируем его как ключевое слово
        if (keyword != m_convention->keywords().end()) {
          return { origami::lex::Token::Keyword, *keyword };
        }

        // Иначе, это именование
        return { origami::lex::Token::Identifier, std::move(word) };

        // Если символ относится к категории 'цифровой символ', определяем полный его идентификатор
      } else if (std::isdigit(m_code[m_current_symbol], std::locale{ "C" })) {
        const auto current_pose_symbol = static_cast<std::string::difference_type>(m_current_symbol);
        auto not_isdigit = std::find_if_not(std::next(m_code.begin(), current_pose_symbol), m_code.end(), [](auto t_ch) {
          return (std::isdigit(t_ch, std::locale{ "C" }) || (t_ch == '.') || (t_ch == '+') || (t_ch == '-'));
        });

        if (not_isdigit != m_code.end()) {
          const auto endOfDigital = std::distance(m_code.begin(), not_isdigit);
          auto digital = m_code.substr(m_current_symbol, static_cast<std::string::size_type>(endOfDigital) - m_current_symbol);
          m_current_symbol = static_cast<decltype(m_current_symbol)>(std::distance(m_code.begin(), not_isdigit));
          return { origami::lex::Token::Literal, std::move(digital) };
        }

        auto digital = m_code.substr(m_current_symbol, m_code.size() - m_current_symbol);
        return { origami::lex::Token::Literal, std::move(digital) };
      } else {
        const auto punctuation = m_convention->punctuation().find(m_code[m_current_symbol]);
        if (punctuation != m_convention->punctuation().end()) {
          // Находим максимульную длину возможной комбинации для текущего символа
          const auto max_combination = std::max_element(punctuation->second.begin(), punctuation->second.end());

          // Приводим максимальную длину к актуальному значению, для того чтобы не выйти за границы массива
          auto max_size = std::clamp<decltype(m_current_symbol)>(max_combination->size(), 0, m_code.size() - m_current_symbol);

          // Производим поиск комбинации с самой длинной возможной конструктуции, до тех пор пока не найдем, либо не выйдем из цикла
          do {
            // Если нашли нужную комбинацию
            if (auto punct = punctuation->second.find(m_code.substr(m_current_symbol, max_size)); punct != punctuation->second.end()) {
              m_current_symbol += max_size;
              return { origami::lex::Token::Punctuator, *punct };
            }
          } while (--max_size != 0);
        }

        const auto operators = m_convention->operators().find(m_code[m_current_symbol]);
        if (operators != m_convention->operators().end()) {
          // Находим максимульную длину возможной комбинации для текущего символа
          const auto max_combination = std::max_element(operators->second.begin(), operators->second.end());

          // Приводим максимальную длину к актуальному значению, для того чтобы не выйти за границы массива
          auto max_size = std::clamp<decltype(m_current_symbol)>(max_combination->size(), 0, m_code.size() - m_current_symbol);
          // Производим поиск комбинации с самой длинной возможной конструктуции, до тех пор пока не найдем, либо не выйдем из цикла
          do {
            // Если нашли нужную комбинацию
            if (auto oper = operators->second.find(m_code.substr(m_current_symbol, max_size)); oper != operators->second.end()) {
              m_current_symbol += max_size;
              return { origami::lex::Token::Operator, *oper };
            }
          } while (--max_size != 0);
        }

        switch (m_code[m_current_symbol]) {
          case '"': {
            if (const auto last_mark = m_code.find_first_of('"', m_current_symbol + 1); last_mark != std::string::npos) {
              m_current_symbol = last_mark + 1;
              return { origami::lex::Token::Literal, m_code.substr(m_current_symbol, last_mark - m_current_symbol) };
            }

            return { origami::lex::Token::Punctuator, std::string{ m_code[m_current_symbol++] } };
          }
          default: {
            throw UnsupportedSymbolError{ fmt::format("Неподдерживаемый символ {0} ", m_code[m_current_symbol]) };
          }
        }
      }
    }

    // Освобождаем память после лексического анализа
    if (!m_code.empty()) {
      m_code.clear();
      m_current_symbol = 0;
    }

    return { Token::Eof, {} };
  }

  /**
   * \brief     Есть ли данные для получения токенов
   *
   * \retval    true - нет данных для получения токенов
   * \retval    false - есть данные для получения токенов
   */
  bool empty() const noexcept
  {
    return m_code.size() == m_current_symbol;
  }

  /**
   * \brief     Привести лексический анализатор в положении по умолчанию
   */
  void clear()
  {
    m_code.clear();
    m_current_symbol = 0;
  }

  /**
   * \brief     Обновить обрабатываемый исходный код программы
   *
   * \param[in] t_code - исходный код программы
   */
  void update(const std::string& t_code)
  {
    m_code = t_code;
    m_current_symbol = 0;
  }

private:
  LexicalConventionImplPtr m_convention;///< Символьная таблица, содержащая информацию о синтаксисе языка программирования
  std::string m_code;///< Исходный код программы
  std::string::size_type m_current_symbol;///<Текущий позиция лексического анализатора
};

template class LexicalAnalyzer<LexicalConventionCpp>;
template class LexicalAnalyzer<LexicalConventionPython>;

}// namespace origami::lex

#endif// ORIGAMI_LEXICAL_ANALYZER_HPP