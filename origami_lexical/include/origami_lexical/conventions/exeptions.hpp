/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация исключений для описания поведения программы на всех стадиях предобаботки исходного кода программы
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_EXEPTIONS_HPP
#define ORIGAMI_EXEPTIONS_HPP

#include <stdexcept>

namespace origami {
/// \brief Общий класс исключения для семантического анализа, этап пострения абстактного синтаксического дерева
class SemanticParsing : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

/// \brief Неподдерживаемая операция в абстрактном синтаксическом дереве
class UnsupportedOperationError : public SemanticParsing
{
  using SemanticParsing::SemanticParsing;

public:
  [[nodiscard]] virtual const char* what() const noexcept override { return "Unsupported operator"; }
};

[[noreturn]] inline void throwUnsupportedOperator() { throw(UnsupportedOperationError{ "" }); }

class InvalidSyntaxError : public SemanticParsing
{
  using SemanticParsing::SemanticParsing;

public:
  [[nodiscard]] virtual const char* what() const noexcept override { return "Invalid syntax"; }
};

[[noreturn]] inline void throwInvalidSyntax() { throw(InvalidSyntaxError{ "" }); }
}// namespace origami

#endif// ORIGAMI_EXEPTIONS_HPP