#include "origami_syntax/parsing/syntax_analysis.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Проверка на суммирвоание выражения Абстрактного синтаксического дерева", "[abstract-syntax-tree-sum]")
{
  origami::parser::AstNodeVisitor visitor;

  SECTION("Проверка корректности работы узлов Абстрактного синтаксического дерева с различными типами данных")
  {
    // TODO: Добавить все типы данных
    origami::parser::ValueNode type_int{ 2345457 };
    REQUIRE(type_int.accept(visitor).has_value());
    REQUIRE(type_int.accept(visitor).type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(type_int.accept(visitor)));
    REQUIRE(std::any_cast<int>(type_int.accept(visitor)) == 2345457);

    origami::parser::ValueNode type_double{ 34.65754657912 };
    REQUIRE(type_double.accept(visitor).has_value());
    REQUIRE(type_double.accept(visitor).type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(type_double.accept(visitor)));
    REQUIRE(std::any_cast<double>(type_double.accept(visitor)) == 34.65754657912);
  }
  SECTION("Суммирование двух целочисленных чисел: 10 + 20.5")
  {
    const auto sum = std::make_shared<origami::parser::SumNode>();
    sum->addLeft(std::make_shared<origami::parser::ValueNode>(10));
    sum->addRight(std::make_shared<origami::parser::ValueNode>(20));
    const std::any result = sum->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 30);
  }
  SECTION("Суммирование числа целого и дробного числа: 10 + 20.5")
  {
    const auto sum = std::make_shared<origami::parser::SumNode>();
    sum->addLeft(std::make_shared<origami::parser::ValueNode>(10));
    sum->addRight(std::make_shared<origami::parser::ValueNode>(20.5));
    const std::any result = sum->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(result));
    REQUIRE(std::any_cast<double>(result) == 30.5);
  }
//  SECTION("Несколько суммирований чисел: 10 + 20 + 30 + 15.5")
//  {
//    const auto first_part = std::make_shared<origami::parser::SumNode>();
//    first_part->addLeft(std::make_shared<origami::parser::ValueNode<int>>(10));
//    first_part->addRight(std::make_shared<origami::parser::ValueNode<int>>(20));
//    REQUIRE(first_part->accept(origami::parser::MultipleData<int>(), visitor));
//    REQUIRE(first_part->accept(origami::parser::MultipleData<int>(), visitor).value() == 30);
//
//    const auto second_part = std::make_shared<origami::parser::SumNode>();
//    second_part->addLeft(first_part);
//    second_part->addRight(std::make_shared<origami::parser::ValueNode<int>>(30));
//    REQUIRE(second_part->accept(origami::parser::MultipleData<int>(), visitor));
//    REQUIRE(second_part->accept(origami::parser::MultipleData<int>(), visitor).value() == 60);
//
//    const auto third_part = std::make_shared<origami::parser::SumNode>();
//    third_part->addLeft(second_part);
//    third_part->addRight(std::make_shared<origami::parser::ValueNode<double>>(15.5));
//    REQUIRE(third_part->accept(origami::parser::MultipleData<int, double>(), visitor));
//    REQUIRE(third_part->accept(origami::parser::MultipleData<int, double>(), visitor).value() == 75.5);
//  }
//  SECTION("Несколько суммирований чисел: 10 + 20 + 30 + 15.5")
//  {
//    const auto first_part = std::make_shared<origami::parser::SumNode>();
//    first_part->addLeft(std::make_shared<origami::parser::ValueNode>(10));
//    first_part->addRight(std::make_shared<origami::parser::ValueNode>(20));
//    REQUIRE(first_part->accept(origami::parser::MultipleData<std::any>(), visitor));
////    REQUIRE(first_part->accept(origami::parser::MultipleData<int>(), visitor).value() == 30);
//
////    const auto second_part = std::make_shared<origami::parser::SumNode>();
////    second_part->addLeft(std::make_shared<origami::parser::ValueNode>(30));
////    second_part->addRight(std::make_shared<origami::parser::ValueNode>(15.5));
//////    REQUIRE(second_part->accept(origami::parser::MultipleData<int, double>(), visitor));
//////    REQUIRE(second_part->accept(origami::parser::MultipleData<int, double>(), visitor).value() == 45.5);
////
////    const auto third_part = std::make_shared<origami::parser::SumNode>();
////    third_part->addLeft(first_part);
////    third_part->addRight(second_part);
////    REQUIRE(third_part->accept(origami::parser::MultipleData<int, double>{}, visitor));
////    REQUIRE(third_part->accept(origami::parser::MultipleData<int, double>{}, visitor).value() == 75.5);
//  }
}