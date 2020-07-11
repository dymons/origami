/*******************************************************************************************************************************************
 * \author      Emelyanov Dmitry <dmitriy.emelyanov.de@gmail.com>
 *
 * \brief       Реализация абстрактного синтаксического дерева
 ******************************************************************************************************************************************/

#ifndef ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP
#define ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP

#include "origami_lexical/conventions/exeptions.hpp"
#include "origami_syntax/parsing/detail/utility.hpp"

#include <any>
#include <memory>

#include <fmt/core.h>

namespace origami::ast {

class AstBase;
class AstNumber;
class AstMathOperator;

class AstVisitor
{
public:
  std::any visit(const AstNumber& t_node);
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
  virtual ~AstBase() = default;

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
  ~AstNode() override = default;

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  AstNode(const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< Определение обработчика класса. Преобразование текущего класса к типу T для вызова обработчика визитора
  std::any accept(AstVisitor& t_visitor) const final;
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
  ~AstNumber() override = default;

  ///< \brief     Пользовательский конструктор. Инициализация полей класса
  explicit AstNumber(std::any t_value);

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  AstNumber(std::any t_value, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< \brief     Инициализировать хранимые данные
  void setValue(std::any t_value);

  ///< \brief     Получить доступ к данным хранимых в классе
  std::any getValue() const;

private:
  std::any m_value;///< Данные хранимые классом
};

template<typename... Ts>
concept Arithmetic = std::conjunction_v<std::is_arithmetic<Ts>...>;

class AstMathOperator : public AstNode<AstMathOperator>
{
public:
  ///< \brief     Конструктор по умолчанию
  AstMathOperator() = default;

  ///< \brief     Деструктор шаблонного базового класса должен быть помечен как виртуальный
  ~AstMathOperator() override = default;

  ///< \brief     Пользовательский конструктор. Инициализация полей класса
  explicit AstMathOperator(std::string t_operator);

  ///< \brief     Пользовательский конструктор для инициализации дочерних узлов базового класса
  explicit AstMathOperator(std::string t_operator, const std::shared_ptr<AstBase>& t_left, const std::shared_ptr<AstBase>& t_right);

  ///< \brief     Инициализировать оператор класса
  void setOperator(std::string t_operator);

  ///< \brief     Получить доступ к оператору класса
  std::string getOperator() const;

  ///< \brief     Арифметическая операция над числами
  template<Arithmetic... Ts>
  auto execute(Ts&&... t_data) const -> typename std::common_type_t<Ts...>;

private:
  std::string m_operator;///< Арифметический оператор
};

template<Arithmetic... Ts>
auto AstMathOperator::execute(Ts&&... t_data) const -> typename std::common_type_t<Ts...>
{
  // clang-format off
  switch (const auto hash = utility::fnv1a::hash(m_operator); hash) {
    case utility::fnv1a::hash("+"): {
      return (t_data + ... + 0);
    }
    case utility::fnv1a::hash("-"): {
      return (t_data - ... - 0);
    }
    case utility::fnv1a::hash("/"): {
      return (t_data / ... / 1);
    }
    case utility::fnv1a::hash("*"): {
      return (t_data * ... * 1);
    }
      [[unlikely]] default: {
      throw UnsupportedOperationError{ fmt::format("Неподдерживаемая операция {0} ", m_operator) };
    }
  }
  // clang-format on
}
}// namespace origami::ast

#endif// ORIGAMI_ABSTRACT_SYNTAX_TREE_HPP