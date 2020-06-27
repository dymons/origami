/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Вспомогательные функции для создания парсера токенов от лексического анализатора
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_UTILITY_HPP
#define ORIGAMI_UTILITY_HPP

#include <string>

namespace origami::utility {
enum class Number : std::uint8_t { Integer, Double, Unknown };

[[nodiscard]] Number isNumber(std::string_view t_str);
}// namespace origami::utility

#endif// ORIGAMI_UTILITY_HPP