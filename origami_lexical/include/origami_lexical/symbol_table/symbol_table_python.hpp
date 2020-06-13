/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYMBOL_TABLE_PYTHON_HPP
#define ORIGAMI_SYMBOL_TABLE_PYTHON_HPP

#include "origami_lexical/symbol_table/symbol_table_impl.hpp"

namespace origami::lex {
class SymbolTablePython final : public SymbolTableImpl
{
public:
  ///< \brief Определяем для Python3 ключевые слова. Пункт 2.3.1.
  const std::set<std::string>& keywords() override;

  ///< \brief Определяем для Python3 операции. Пункт 2.5
  const std::map<char, std::set<std::string>>& operators() override;

  ///< \brief Определяем для Python3 знаки пунктуации. Пункт 2.6
  const std::map<char, std::set<std::string>>& punctuation() override;
};
}// namespace origami::lex

#endif// ORIGAMI_SYMBOL_TABLE_PYTHON_HPP