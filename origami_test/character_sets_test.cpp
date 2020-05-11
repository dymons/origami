#include "origami_lexical/conventions/character_sets.hpp"

#include <gtest/gtest.h>

TEST(LexicalConventions, DISABLED_CharacterSet)
{
  // Проверка 26 букв английского алфавита на принадлежность к Character sets, в нижнем регистре.
  for (char character = 'a'; character != '{'; ++character)
  {
    ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  // Проверка 26 букв английского алфавита на принадлежность к Character sets, в верхнем регистре.
  for (char character = 'A'; character != '['; ++character)
  {
    ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  // Проверка цифровой системы знаков на принадлежность к Character sets, 0 ... 9
  for (char character = '0'; character != ':'; ++character)
  {
    ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  // Проверка символов, относящихся к пунктуации, на принадлежность к Character set
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('_'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('{'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('}'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('['));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(']'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('#'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('('));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(')'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('<'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('>'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('%'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(':'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(';'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('.'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('?'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('*'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('+'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('-'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('/'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('^'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('&'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('|'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('~'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('!'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('='));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(','));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('\\'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('"'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('`'));

  // Проверка символов, относящихся к категории C locale, на принадлежность к Character set
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet(' '));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('\t'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('\v'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('\f'));
  ASSERT_TRUE(origami::lex::charset::isBasicSourceCharacterSet('\n'));

  // Остальное не должно относится к Character set
  for (auto character = static_cast<unsigned char>(0); character != static_cast<unsigned char>(9); ++character)
  {
    ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  for (auto character = static_cast<unsigned char>(13); character != static_cast<unsigned char>(32); ++character)
  {
    ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  for (auto character = static_cast<unsigned char>(128); character != static_cast<unsigned char>(255); ++character)
  {
    ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet(character));
  }

  ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet('@'));
  ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet(static_cast<unsigned char>(127))); // Delete
  ASSERT_FALSE(origami::lex::charset::isBasicSourceCharacterSet(static_cast<unsigned char>(255))); // nbsp
}