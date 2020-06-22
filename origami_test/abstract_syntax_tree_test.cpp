#include "origami_syntax/parsing/syntax_analysis.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Проверка на суммирвоание выражения Абстрактного синтаксического дерева", "[abstract-syntax-tree-sum]")
{
  origami::parser::AstNodeVisitor visitor;

  SECTION("Проверка корректности работы узлов Абстрактного синтаксического дерева с различными типами данных")
  {
    origami::parser::ValueNode<int> type_int{ 0 };
    REQUIRE(type_int.accept(origami::parser::SingleData<int>{}, visitor) == 0);

    origami::parser::ValueNode<double> type_double{ 0.0 };
    REQUIRE(type_double.accept(origami::parser::SingleData<double>{}, visitor) == 0.0);
  }
  SECTION("Суммирование двух целочисленных чисел: 10 + 20")
  {
    const auto sum_node = std::make_shared<origami::parser::SumNode>();
    sum_node->addLeft(std::make_shared<origami::parser::ValueNode<int>>(10));
    sum_node->addRight(std::make_shared<origami::parser::ValueNode<int>>(20));
    const std::optional<int> res = sum_node->accept(origami::parser::SingleData<int>{}, visitor);
    REQUIRE(res);
    REQUIRE(res.value() == 30);
  }
  SECTION("Суммирование целого числа с числом с плавающей точкой: 10 + 20")
  {
    const auto sum_node = std::make_shared<origami::parser::SumNode>();
    sum_node->addLeft(std::make_shared<origami::parser::ValueNode<int>>(10));
    sum_node->addRight(std::make_shared<origami::parser::ValueNode<double>>(20.5));
    const std::optional<double> res = sum_node->accept(origami::parser::SingleData<double>(), visitor);
    REQUIRE(res);
    REQUIRE(res.value() == 30.5);
  }
  SECTION("Несколько суммирований чисел: 10 + 20 + 15.5")
  {
    const auto sum_node_int = std::make_shared<origami::parser::SumNode>();
    sum_node_int->addLeft(std::make_shared<origami::parser::ValueNode<int>>(10));
    sum_node_int->addRight(std::make_shared<origami::parser::ValueNode<int>>(20));
    const std::optional<int> res_int = sum_node_int->accept(origami::parser::SingleData<int>(), visitor);
    REQUIRE(res_int);
    REQUIRE(res_int.value() == 30);

    const auto sum_node_double = std::make_shared<origami::parser::SumNode>();
    sum_node_double->addLeft(sum_node_int);
    sum_node_double->addRight(std::make_shared<origami::parser::ValueNode<double>>(15.5));
    std::optional<double> res_double = sum_node_double->accept(origami::parser::SingleData<double>(), visitor);
    REQUIRE(res_double);
    REQUIRE(res_double.value() == 45.5);
  }
}