/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит описание структур и взаимосвязи между ними, для преобразования исходного кода программы в token'ы
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_TOKENS_HPP
#define ORIGAMI_TOKENS_HPP

#include <cstdint>

namespace origami::lex::pptoken {
enum class PreprocessingToken : std::uint8_t {
    HeaderName,
    Identifier,
    PPNumber,
    CharacterLiteral,
    UserDefinedCharacterLiteral,
    StringLiteral,
    UserDefinedStringLiteral,
    PreprocessingOpOrPunc,
    Unknown
};
} // namespace origami::lex::pptoken

namespace origami::lex::token {
enum class Token : std::uint8_t {
    Identifier,
    Keyword,
    Literal,
    Operator,
    Punctuator
};
} // namespace origami::lex::token

namespace origami::lex::digraph {
struct AlternativeTokens {
  enum class Tokens : std::uint8_t {
      CurlyBracketLeft, CurlyBracketRight,
      SquareBracketLeft, SquareBracketRight,
      NumberSign, NumberSignDouble,
      And, Bitor, Or, Xor, Compl, Bitand,
      AndEq, OrEq, XorEq, Not, NotEq
  };
};
} // namespace origami::lex::digraph

#endif // ORIGAMI_TOKENS_HPP