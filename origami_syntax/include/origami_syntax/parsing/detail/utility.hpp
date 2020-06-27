/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_UTILITY_HPP
#define ORIGAMI_UTILITY_HPP

namespace origami::utility {
enum class Number : std::uint8_t { Integer, Double, Unknown };

// TODO: Перенести в cpp
inline Number isNumber(const std::string& t_str)
{
  bool is_digit = std::find_if(t_str.begin(), t_str.end(), [](char t_c) { return !(std::isdigit(t_c) != 0 || t_c == '.'); }) == t_str.end();

  if (t_str.empty() || !is_digit) { return Number::Unknown; }

  return t_str.find('.') != std::string::npos ? Number::Double : Number::Integer;
}

}// namespace origami::utility

#endif// ORIGAMI_UTILITY_HPP