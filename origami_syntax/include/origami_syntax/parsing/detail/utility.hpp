/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Вспомогательные функции для создания парсера токенов от лексического анализатора
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_UTILITY_HPP
#define ORIGAMI_UTILITY_HPP

#include <cstdint>
#include <algorithm>
#include <iterator>
#include <ranges>

namespace origami::utility {
enum class Number : std::uint8_t { Integer, Double, Unknown };

template<typename T> requires std::bidirectional_iterator<typename T::iterator>[[nodiscard]] Number isNumber(const T& t_str)
{
  // Проверяем, что все символы в строке принадлежат к типу чисел
  bool is_digit = std::ranges::all_of(t_str, [](char t_c) { return std::isdigit(t_c) != 0 || t_c == '.'; });

  // Если строка пустая или данные в строке не могут быть приведены к числовому типу
  if (t_str.empty() || !is_digit) { return Number::Unknown; }

  // Если в строке присутствует точка, то данные принадлежат к классу floating point
  return std::ranges::find(t_str, '.') != std::ranges::end(t_str) ? Number::Double : Number::Integer;
}
}// namespace origami::utility

#endif// ORIGAMI_UTILITY_HPP