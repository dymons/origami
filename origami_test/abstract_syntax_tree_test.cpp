#include "origami_syntax/parsing/syntax_analysis.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Проверка на суммирвоание выражения Абстрактного синтаксического дерева", "[abstract-syntax-tree-sum]")
{
  origami::parser::AstNodeVisitor visitor;

  SECTION("Суммирование двух целочисленных чисел")
  {
    const auto sum_node = std::make_shared<origami::parser::SumNode>();
    sum_node->addLeft(std::make_shared<origami::parser::ValueNode<int>>(10));
    sum_node->addRight(std::make_shared<origami::parser::ValueNode<int>>(20));
    std::optional<int> res = sum_node->accept(origami::parser::Data<int>(), visitor);
    REQUIRE(res);
    REQUIRE(res.value() == 30);
  }
}