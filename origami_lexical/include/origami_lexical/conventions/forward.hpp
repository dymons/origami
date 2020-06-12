/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_FORWARD_HPP
#define ORIGAMI_FORWARD_HPP

#include <memory>
#include <string>

namespace origami::lex {
class SymbolTableImpl;
using SymbolTableImplPtr = std::shared_ptr<SymbolTableImpl>;
using SymbolTableImplConstPtr = std::shared_ptr<const SymbolTableImpl>;

using Lexeme = std::string;
}// namespace origami::lex

#endif// ORIGAMI_FORWARD_HPP