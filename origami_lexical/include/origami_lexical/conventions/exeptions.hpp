/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_EXEPTIONS_HPP
#define ORIGAMI_EXEPTIONS_HPP

#include <stdexcept>

namespace origami {
class OrigamiError : public std::runtime_error
{
  using std::runtime_error::runtime_error;
};

class InvalidInputError : public OrigamiError
{
  using OrigamiError::OrigamiError;
};
}// namespace origami

#endif// ORIGAMI_EXEPTIONS_HPP