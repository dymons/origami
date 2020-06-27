/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Вспомогательные функции для создания парсера токенов от лексического анализатора
 ******************************************************************************************************************************************/

#include "origami_syntax/parsing/detail/utility.hpp"

#include <algorithm>

namespace origami::utility {
Number isNumber(const std::string_view t_str)
{
  // Проверяем, что все символы в строке принадлежат к типу чисел
  bool is_digit = std::find_if(t_str.begin(), t_str.end(), [](char t_c) { return !(std::isdigit(t_c) != 0 || t_c == '.'); }) == t_str.end();

  // Если строка пустая или данные в строке не могут быть приведены к числовому типу
  if (t_str.empty() || !is_digit) { return Number::Unknown; }

  // Если в строке присутствует точка, то данные принадлежат к классу floating point
  return t_str.find('.') != std::string::npos ? Number::Double : Number::Integer;
}
}// namespace origami::utility