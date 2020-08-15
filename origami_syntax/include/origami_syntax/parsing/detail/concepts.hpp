/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Вспомогательные Concepts
 *
 * \details     https://en.cppreference.com/w/cpp/language/constraints
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_CONCEPTS_HPP
#define ORIGAMI_CONCEPTS_HPP

#include <type_traits>

namespace origami::concepts {
template<typename... Ts>
concept Arithmetic = std::conjunction_v<std::is_arithmetic<Ts>...>;
}// namespace origami::concepts

#endif// ORIGAMI_CONCEPTS_HPP