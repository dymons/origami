/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит описание структур и взаимосвязи между ними, для преобразования исходного кода программы в token'ы
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_TOKENS_HPP
#define ORIGAMI_TOKENS_HPP

#include <cstdint>

namespace origami::lex {
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

enum class Token : std::uint8_t {
    Identifier,
    Keyword,
    Literal,
    Operator,
    Punctuator
};

struct AlternativeTokens {
  enum class Digraph : std::uint8_t {
      CurlyBracketLeft, CurlyBracketRight,
      SquareBracketLeft, SquareBracketRight,
      NumberSign, NumberSignDouble,
      And, Bitor, Or, Xor, Compl, Bitand,
      AndEq, OrEq, XorEq, Not, NotEq
  };
};

enum class Keywords : std::uint8_t {
    Alignas, ConstCast, For, Public, ThreadLocal, Alignof, Continue, Friend, Register, Throw, Asm, Decltype, Goto, ReinterpretCast, True,
    Auto, Default, If, Requires, Try, Bool, Delete, Inline, Return, Typedef, Break, Do, Int, Short, Typeid, Case, Double, Long, Signed,
    Typename, Catch, DynamicCast, Mutable, Sizeof, Union, Char, Else, Namespace, Static, Unsigned, Char16_t, Enum, New, StaticAssert,
    Using, Char32_t, Explicit, Noexcept, StaticCast, Virtual, Class, Export, Nullptr, Struct, Void, Concept, Extern, Operator, Switch,
    Volatile, Const, False, Private, Template, Wchar_t, Constexpr, Float, Protected, This, While
};
} // namespace origami::lex

#endif // ORIGAMI_TOKENS_HPP