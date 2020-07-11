/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP
#define ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP

#include "origami_lexical/conventions/exeptions.hpp"
#include "origami_syntax/parsing/detail/utility.hpp"
#include "origami_syntax/parsing/detail/concepts.hpp"

#include <any>
#include <memory>

#include <fmt/core.h>

namespace origami::ast {

class AstBase;
class AstNumber;
class AstMathOperator;

///< \brief      Абстрактное синтаксическое дерево основано на паттерне Visitor идиомы CRTP
class AstVisitor final
{
public:
  ///< \brief    Обработка узла AstNumber дерева. Возвращает занчение, которое хранится в AstNumber
  std::any visit(const AstNumber& t_node);

  ///< \brief    Обработка узла AstMathOperator дерева. Выполняет арифметическую операцию и возвращает результат
  std::any visit(const AstMathOperator& t_node);
};

///< \brief       Базовый класс для всех узлов Абстрактного синтаксического дерева. Используется для восходящего привидения типа
class AstBase
{
public:
  ///< \brief     Конструктор по умолчанию
  AstBase();

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов
  AstBase(const std::shared_ptr<AstBase>& t_left_child, const std::shared_ptr<AstBase>& t_right_child);

  ///< \brief     Виртуальный деструктор
  virtual ~AstBase() noexcept = default;

  ///< \brief     Обрабочик класса
  virtual std::any accept(AstVisitor& t_visitor) const = 0;

  ///< \brief     Задание левого узла дерева
  void setLeftChild(const std::shared_ptr<AstBase>& t_child);

  ///< \brief     Задание правого узла дерева
  void setRightChild(const std::shared_ptr<AstBase>& t_child);

  ///< \brief     Получить доступ к левому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstBase> getLeftChild() const;

  ///< \brief     Получить доступ к правому дочернему узлу текущего узла
  [[nodiscard]] std::shared_ptr<AstBase> getRightChild() const;

private:
  std::shared_ptr<AstBase> m_left_child;///< Левый дочерний узел
  std::shared_ptr<AstBase> m_right_child;///< Правый дочерний узел
};

template<typename T>
class AstNode : public AstBase
{
public:
  ///< \brief     Конструктор по умолчанию
  AstNode() = default;

  ///< \brief     Деструктор базового класса должен быть помечен как виртуальный
  ~AstNode() noexcept override = default;

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  AstNode(const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< Определение обработчика класса. Преобразование текущего класса к типу T для вызова обработчика визитора
  [[nodiscard]] std::any accept(AstVisitor& t_visitor) const final;
};

template<typename T>
AstNode<T>::AstNode(const std::shared_ptr<AstBase>& t_left_child, const std::shared_ptr<AstBase>& t_right_child)
  : AstBase(t_left_child, t_right_child)
{}

template<typename T>
std::any AstNode<T>::accept(AstVisitor& t_visitor) const
{
  // Нисходящее привидение к типу производного класса
  if (const auto* upcast = dynamic_cast<const T* const>(this); upcast) {
    return t_visitor.visit(*upcast);
  }

  return {};
}

class AstNumber : public AstNode<AstNumber>
{
public:
  ///< \brief     Конструктор по умолчанию
  AstNumber() = default;

  ///< \brief     Деструктор шаблонного базового класса должен быть помечен как виртуальный
  ~AstNumber() noexcept override = default;

  ///< \brief     Пользовательский конструктор. Инициализация полей класса
  explicit AstNumber(std::any t_value);

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  AstNumber(std::any t_value, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< \brief     Инициализировать хранимые данные
  void setValue(std::any t_value);

  ///< \brief     Получить доступ к данным хранимых в классе
  [[nodiscard]] std::any getValue() const;

private:
  std::any m_value;///< Данные хранимые классом
};

class AstMathOperator : public AstNode<AstMathOperator>
{
public:
  ///< \brief     Конструктор по умолчанию
  AstMathOperator() = default;

  ///< \brief     Деструктор шаблонного базового класса должен быть помечен как виртуальный
  ~AstMathOperator() noexcept override = default;

  ///< \brief     Пользовательский конструктор. Инициализация полей класса
  explicit AstMathOperator(std::string t_operator);

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  explicit AstMathOperator(std::string t_operator, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< \brief     Инициализировать оператор класса
  void setOperator(std::string t_operator);

  ///< \brief     Получить доступ к оператору класса
  [[nodiscard]] std::string getOperator() const;

  ///< \brief     Арифметическая операция над числами
  template<concepts::Arithmetic... Ts>
  [[nodiscard]] auto execute(Ts&&... t_data) const -> typename std::common_type_t<Ts...>;

private:
  std::string m_operator;///< Арифметический оператор
};
}// namespace origami::ast

#endif// ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP