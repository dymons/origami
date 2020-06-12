/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Описывает базовую структуру языка программирования. Из каких ключевых слов, операторов и знаков пунктуации он состоит.
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYMBOL_TABLE_IMPL_HPP
#define ORIGAMI_SYMBOL_TABLE_IMPL_HPP

#include "origami_lexical/conventions/forward.hpp"
#include "origami_lexical/conventions/tokens.hpp"

#include <set>
#include <map>
#include <string>

namespace origami::lex {
class SymbolTableImpl
{
public:
  SymbolTableImpl() = default;

  virtual ~SymbolTableImpl() = default;

  SymbolTableImpl(const SymbolTableImpl&) = default;

  SymbolTableImpl& operator=(const SymbolTableImpl&) = default;

  SymbolTableImpl(SymbolTableImpl&&) = default;

  SymbolTableImpl& operator=(SymbolTableImpl&&) = default;

  /**
   * \brief          Ключевые слова для языка программирования
   *
   * \details        Стандарт для языка программирования должен содержать графу с Lexical conventions, в которой описаны
   *                 все зарезервированные слова.
   *
   * \return         Возвращает все ключевые слова, которые относятся к языку программирования
   */
  virtual std::set<std::string> keywords() = 0;

  /**
   * \brief          Операторы для языка программирования
   *
   * \details        Стандарт для языка программирования должен содержать графу с Lexical conventions, в которой описаны
   *                 все возможные операторы.
   *
   *                 Ключем выступает первый символ оператора. В качестве значения указываются всевозможные комбинации для данного символа.
   *                 К примеру, { key: '-', value: { "-", "-=", "--" } }
   *
   * \return         Возвращает все операторы, которые относятся к языку программирования
   */
  virtual std::map<char, std::set<std::string>> operators() = 0;

  /**
   * \brief          Пунктуация для языка программирования
   *
   * \details        Стандарт для языка программирования должен содержать графу с Lexical conventions, в которой описаны
   *                 вся возможная пунктуация.
   *
   *                 Ключем выступает первый символ знака пунктуации. В качестве значения указываются всевозможные комбинации для данного
   *                 знака. К примеру, { key: '-', { value: "->", "->*" } }
   *
   * \return         Возвращает все пунктуационные знаки, которые относятся к языку программирования
   */
  virtual std::map<char, std::set<std::string>> punctuation() = 0;
};
}// namespace origami::lex

#endif// ORIGAMI_SYMBOL_TABLE_IMPL_HPP