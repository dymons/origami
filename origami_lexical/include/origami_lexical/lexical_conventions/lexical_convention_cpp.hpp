/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Символьная таблица для С++ 17 стандарта
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_LEXICAL_CONVENTION_CPP_HPP
#define ORIGAMI_LEXICAL_CONVENTION_CPP_HPP

#include "origami_lexical/lexical_conventions/lexical_convention_impl.hpp"

namespace origami::lex {
class LexicalConventionCpp final : public LexicalConventionImpl
{
public:
  ///< \brief Определяем для С++17 ключевые слова. 5 глава, пункт 5.11
  const std::set<std::string>& keywords() override;

  ///< \brief Определяем для С++17 операции. 5 глава, пункт 5.12
  const std::map<char, std::set<std::string>>& operators() override;

  ///< \brief Определяем для С++17 знаки пунктуации. 5 глава, пункт 5.12
  const std::map<char, std::set<std::string>>& punctuation() override;
};
}// namespace origami::lex

#endif// ORIGAMI_LEXICAL_CONVENTION_CPP_HPP