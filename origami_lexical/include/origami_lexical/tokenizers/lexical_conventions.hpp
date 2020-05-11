/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит лексические анализаторы для С++17
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_CONVENTIONS_HPP
#define ORIGAMI_LEXICAL_CONVENTIONS_HPP

#include "origami_lexical/conventions/tokens.hpp"

#include <deque>
#include <set>
#include <string>

#include <iostream>

namespace origami::lex {

/**
  * \brief           Аналитического разбор входной последовательности символов на распознанные группы — лексемы — с целью получения
  *                  на выходе идентифицированных последовательностей, называемых «токенами». Аналитический разбор основан на анализе
  *                  стандарта С++17.
  */
class LexicalConventions {
  public:
    /**
      * \brief          Формирует последовательность token'ов на основе исходный кода программы
      *
      * \param[in]      t_code - исходный код программы
      *
      * \return         Последовательность token'ов
      */
    std::deque<std::pair<origami::lex::Token, std::string>> getTokens(const std::string& t_code);

  private:
    /**
      * \brief          Создание ключевых слов С++17
      *
      * \return         Все ключевые слова С++17, которые описаны в пункте 5.11 таблица 5
      */
    static const std::set<std::string>& keywords()
    {
      static const std::set<std::string> k { "int",
                                             "return" };
      return k;
    }

    static const std::set<std::string>& preprocessorsKeywords()
    {
      static const std::set<std::string> p { "include",
                                             "if",
                                             "ifdef",
                                             "ifndef",
                                             "elif",
                                             "else",
                                             "endif",
                                             "error",
                                             "define",
                                             "line",
                                             "undef",
                                             "using" };

      return p;
    }

    const std::set<std::string>& m_keywords = keywords(); ///< Ключевые слова С++17
    const std::set<std::string>& m_preprocessorKeywords = preprocessorsKeywords(); ///< Ключевые слова для препроцессора
};

} // namespace origami::lex

#endif //ORIGAMI_LEXICAL_CONVENTIONS_HPP