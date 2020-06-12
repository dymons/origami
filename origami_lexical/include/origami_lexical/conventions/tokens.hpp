/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит описание структур и взаимосвязи между ними, для преобразования исходного кода программы в token'ы
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_TOKENS_HPP
#define ORIGAMI_TOKENS_HPP

#include <cstdint>

namespace origami::lex {
enum class Token : std::uint8_t { Identifier, Keyword, Literal, Operator, Punctuator, KeywordPreprocessor };
}// namespace origami::lex

#endif// ORIGAMI_TOKENS_HPP