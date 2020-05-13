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
    static const std::set<std::string>& keywords()
    {
      static const std::set<std::string> k {
        "alignas", "alignof", "asm", "auto", "bool", "break", "case", "catch", "char", "char16_t", "char32_t", "class", "concept", "const",
        "constexpr", "const_cast", "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit",
        "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", "new",
        "noexcept", "nullptr", "operator", "private", "protected", "public", "register", "reinterpret_cast", "requires", "return",
        "short", "signed", "sizeof", "static", "static_assert", "static_cast", "switch", "template", "this", "thread_local", "throw",
        "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while" };

      return k;
    }

    static const std::set<std::string>& preprocessorsKeywords()
    {
      static const std::set<std::string> p {
        "include", "if", "ifdef", "ifndef",
        "elif", "else", "endif", "error",
        "define", "line", "undef", "using" };

      return p;
    }

    static const std::map<char, std::set<std::string>>& operatorsAndPunctuators()
    {
      static const std::map<char, std::set<std::string>> op_or_punc {
        {'<', {"<", "<:", "<%", "<=", "<=>", "<<", "<<="}},
        {'>', {">", ">=", ">>", ">>="}}
      };

      return op_or_punc;
    }

    const std::set<std::string>& m_keywords = keywords(); ///< Ключевые слова С++17
    const std::set<std::string>& m_preprocessorKeywords = preprocessorsKeywords(); ///< Ключевые слова для препроцессора
    const std::map<char, std::set<std::string>>& m_operators = operatorsAndPunctuators(); ///< Все операторы и пунктуация
};

} // namespace origami::lex

#endif //ORIGAMI_LEXICAL_CONVENTIONS_HPP