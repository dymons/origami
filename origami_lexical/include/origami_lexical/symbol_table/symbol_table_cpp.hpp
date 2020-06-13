/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_SYMBOL_TABLE_CPP_HPP
#define ORIGAMI_SYMBOL_TABLE_CPP_HPP

#include "origami_lexical/symbol_table/symbol_table_impl.hpp"

namespace origami::lex {
class SymbolTableCpp final : public SymbolTableImpl
{
public:
  ///< \brief Определяем для С++17 ключевые слова. 5 глава, пункт 5.11
  [[nodiscard]] const std::set<std::string>& keywords() override;

  ///< \brief Определяем для С++17 операции. 5 глава, пункт 5.12
  [[nodiscard]] const std::map<char, std::set<std::string>>& operators() override;

  ///< \brief Определяем для С++17 знаки пунктуации. 5 глава, пункт 5.12
  [[nodiscard]] const std::map<char, std::set<std::string>>& punctuation() override;
};
}// namespace origami::lex

#endif// ORIGAMI_SYMBOL_TABLE_CPP_HPP