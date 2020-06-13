/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит лексический анализатор для С++17
 *
 * \todo        Посмотреть в сторону регулярных выражений для распознание групп — лексем
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_CONVENTIONS_HPP
#define ORIGAMI_LEXICAL_CONVENTIONS_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/conventions/tokens.hpp"

#include <deque>
#include <map>
#include <set>
#include <string>

namespace origami::lex {

/**
 * \brief           Аналитического разбор входной последовательности символов на распознанные группы — лексемы — с целью получения
 *                  на выходе идентифицированных последовательностей, называемых «токенами». Аналитический разбор основан на анализе
 *                  стандарта С++17.
 */
class LexicalConventions
{
public:
  /**
   * \brief          Формирует последовательность token'ов на основе исходный кода программы
   *
   * \param[in]      t_code - исходный код программы
   *
   * \return         Последовательность token'ов
   */
  [[nodiscard]] std::deque<std::pair<origami::lex::Token, std::string>> getTokens(const std::string& t_code);

private:
  /**
   * \brief          Создание ключевых слов С++17
   *
   * \return         Все ключевые слова С++17, которые описаны в пункте 5.11 таблица 5
   */
  [[nodiscard]] static const std::set<std::string>& keywords()
  {
    static const std::set<std::string> K{ "alignas",
      "alignof",
      "asm",
      "auto",
      "bool",
      "break",
      "case",
      "catch",
      "char",
      "char16_t",
      "char32_t",
      "class",
      "concept",
      "const",
      "constexpr",
      "const_cast",
      "continue",
      "decltype",
      "default",
      "delete",
      "do",
      "double",
      "dynamic_cast",
      "else",
      "enum",
      "explicit",
      "export",
      "extern",
      "false",
      "float",
      "for",
      "friend",
      "goto",
      "if",
      "inline",
      "int",
      "long",
      "mutable",
      "namespace",
      "new",
      "noexcept",
      "nullptr",
      "operator",
      "private",
      "protected",
      "public",
      "register",
      "reinterpret_cast",
      "requires",
      "return",
      "short",
      "signed",
      "sizeof",
      "static",
      "static_assert",
      "static_cast",
      "switch",
      "template",
      "this",
      "thread_local",
      "throw",
      "true",
      "try",
      "typedef",
      "typeid",
      "typename",
      "union",
      "unsigned",
      "using",
      "virtual",
      "void",
      "volatile",
      "wchar_t",
      "while",

      // Стандарт кодирования 5.12
      "and",
      "or",
      "xor",
      "not",
      "bitand",
      "bitor",
      "compl",
      "and_eq",
      "or_eq",
      "xor_eq",
      "not_eq" };

    return K;
  }

  [[nodiscard]] static const std::map<char, std::set<std::string>>& operators()
  {
    // Стандарт кодирования 5.12
    static const std::map<char, std::set<std::string>> Op{ { '+', { "+", "+=", "++" } },
      { '-', { "-", "-=", "--" } },
      { '*', { "*", "*=" } },
      { '/', { "/", "/=" } },
      { '%', { "%", "%=", "%:", "%:%:" } },
      { '=', { "=", "==" } },
      { '!', { "!", "!=" } },
      { '>', { ">", ">=", ">>", ">>=" } },
      { '<', { "<", "<=", "<=>", "<<", "<<=" } },
      { '&', { "&", "&=", "&&" } },
      { '|', { "|", "|=", "||" } },
      { '^', { "^", "^=" } },
      { '?', { "?" } },
      { '~', { "~" } },
      { '#', { "#", "##" } } };

    return Op;
  }

  [[nodiscard]] static const std::map<char, std::set<std::string>>& punctuation()
  {
    // Стандарт кодирования 5.12
    static const std::map<char, std::set<std::string>> Punc{ { '<', { "<:", "<%" } },
      { '%', { "%>", "%:", "%:%" } },
      { '-', { "->", "->*" } },
      { '{', { "{" } },
      { '}', { "}" } },
      { '[', { "[" } },
      { ']', { "]" } },
      { '(', { "(" } },
      { ')', { ")" } },
      { ':', { ":", ":>", "::" } },
      { ';', { ";" } },
      { ';', { ";" } },
      { '.', { ".", "...", ".*" } },
      { ',', { "," } } };

    return Punc;
  }

  // TODO: Сделать независимфм от SymbolTable, посмотреть паттерны. Так как можно сделать для других языков программирования. Python,
  // к примеру.
  // TODO: Перенести в Symbol Table. http://cpp-reference.ru/patterns/creational-patterns/singleton/
  const std::set<std::string>& m_keywords = keywords();///< Ключевые слова С++17
  const std::map<char, std::set<std::string>>& m_operators = operators();///< Все операторы
  const std::map<char, std::set<std::string>>& m_punctuation = punctuation();///< Вся пунктуация
};

}// namespace origami::lex

#endif// ORIGAMI_LEXICAL_CONVENTIONS_HPP