#include "origami_lexical/conventions/character_sets.hpp"

#include <cctype>

namespace origami::lex::charset {
bool isBasicSourceCharacterSet(char t_character) noexcept
{
  // Заданы 96 символов, описанных в стандарте С++, как принадлежащих к группе basic source character set
  switch (t_character) {
    case 'a' ... 'z': [[fallthrough]];
    case 'A' ... 'Z': [[fallthrough]];
    case ' ' ... '/': [[fallthrough]];
    case ':' ... '?': [[fallthrough]];
    case '[' ... '`': [[fallthrough]];
    case '{' ... '~': [[fallthrough]];
    case '0' ... '9': [[fallthrough]];
    case '\t' ... '\f': break;
    default:
      return false;
  }

  return true;
}
} // namespace origami::lex::charset