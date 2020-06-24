/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Описание поддерживаемых сущностей в библиотеке
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_FORWARD_HPP
#define ORIGAMI_FORWARD_HPP

#ifdef _DEBUG
#define ORIGAMI_DEBUG
#endif

#ifdef ORIGAMI_DEBUG
#include <cassert>
#include <iostream>
#endif

#include <memory>
#include <string>

namespace origami::lex {
class LexicalConventionImpl;
using LexicalConventionImplPtr = std::shared_ptr<LexicalConventionImpl>;
using LexicalConventionImplConstPtr = std::shared_ptr<const LexicalConventionImpl>;

using Lexeme = std::string;
}// namespace origami::lex

#endif// ORIGAMI_FORWARD_HPP