/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит лексические анализаторы для С++17
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_CONVENTIONS_HPP
#define ORIGAMI_LEXICAL_CONVENTIONS_HPP

#include "origami_lexical/conventions/tokens.hpp"

#include <deque>
#include <map>
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
    static const std::map<std::string, origami::lex::Keywords>& keywords()
    {
      static const std::map<std::string, origami::lex::Keywords> k {
        { "int",    origami::lex::Keywords::Int },
        { "return", origami::lex::Keywords::Return }
      };

      return k;
    }

    const std::map<std::string, origami::lex::Keywords>& m_keywords = keywords(); ///< Ключевые слова С++17
};

} // namespace origami::lex

#endif //ORIGAMI_LEXICAL_CONVENTIONS_HPP