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

template<typename T>
[[nodiscard]] auto isNumber(T&& t_sequence) -> Number
{
  // Проверяем, что все символы в строке принадлежат к типу чисел
  const auto is_digit = std::ranges::all_of(t_sequence, [](const auto t_c) { return (std::isdigit(t_c) != 0) || (t_c == '.'); });

  // Если строка пустая или данные в строке не могут быть приведены к числовому типу
  // clang-format off
  if (std::ranges::empty(t_sequence) || !is_digit) [[unlikely]] {
    return Number::Unknown;
  }
  // clang-format on

  // Если в строке присутствует точка, то данные принадлежат к классу floating point
  return std::ranges::find(t_sequence, '.') != std::ranges::end(t_sequence) ? Number::Double : Number::Integer;
}

namespace fnv1a {
  // see: https://github.com/ChaiScript/ChaiScript/blob/develop/include/chaiscript/utility/hash.hpp
  // see: https://ru.wikipedia.org/wiki/FNV
  template<typename Itr>
  static constexpr std::uint32_t hash(Itr t_begin, Itr t_end) noexcept
  {
    std::uint32_t hash = 0x811c9dc5;

    while (t_begin != t_end) {
      hash = (hash ^ (*t_begin)) * 0x01000193;
      ++t_begin;
    }

    return hash;
  }

  template<size_t N>
  static constexpr std::uint32_t hash(const char (&t_str)[N]) noexcept
  {
    return hash(std::begin(t_str), std::end(t_str) - 1);
  }

  static constexpr std::uint32_t hash(const std::string_view& t_sv) noexcept
  {
    return hash(t_sv.begin(), t_sv.end());
  }

  static inline std::uint32_t hash(const std::string& t_str) noexcept
  {
    return hash(t_str.begin(), t_str.end());
  }
}// namespace fnv1a
}// namespace origami::utility

#endif// ORIGAMI_UTILITY_HPP