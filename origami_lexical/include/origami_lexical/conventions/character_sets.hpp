/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Содержит вспомогательные функции для проверки валидности исходного кода программы на С++
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_CHARACTER_SETS_HPP
#define ORIGAMI_CHARACTER_SETS_HPP

namespace origami::lex::charset {
/**
  * \brief          Проверка символа на принадлежность его к категории basic source character set
  *
  * \details        Данная часть описана в стандарте С++, пункт 5.3
  *
  * \param[in]      t_character - символ
  *
  * \retval         true - Символ принадлежит к группе basic source character set
  * \retval         false - Символ не принадлежит к группе basic source character set
  */
[[nodiscard]] bool isBasicSourceCharacterSet(unsigned char t_character) noexcept;
} // namespace origami::lex::charset

#endif // ORIGAMI_CHARACTER_SETS_HPP