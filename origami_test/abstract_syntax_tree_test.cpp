#include "origami_syntax/parsing/syntax_analysis.hpp"

#include <catch2/catch.hpp>

#include <memory>

TEST_CASE("Проверка на суммирвоание выражения Абстрактного синтаксического дерева", "[abstract-syntax-tree-sum]")
{
  origami::ast::AstVisitor visitor;

  SECTION("Проверка корректности работы узлов Абстрактного синтаксического дерева с различными типами данных")
  {
    origami::ast::AstNumber type_int{ 2345457 };
    REQUIRE(type_int.accept(visitor).has_value());
    REQUIRE(type_int.accept(visitor).type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(type_int.accept(visitor)));
    REQUIRE(std::any_cast<int>(type_int.accept(visitor)) == 2345457);

    origami::ast::AstNumber type_double{ 34.65754657912 };
    REQUIRE(type_double.accept(visitor).has_value());
    REQUIRE(type_double.accept(visitor).type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(type_double.accept(visitor)));
    REQUIRE(std::any_cast<double>(type_double.accept(visitor)) == 34.65754657912);
  }
  SECTION("Суммирование двух целочисленных чисел: 10 + 20.5")
  {
    const auto sum = std::make_shared<origami::ast::AstMathOperator>("+");
    sum->setLeftChild(std::make_shared<origami::ast::AstNumber>(10));
    sum->setRightChild(std::make_shared<origami::ast::AstNumber>(20));
    const std::any result = sum->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 30);
  }
  SECTION("Суммирование числа целого и дробного числа: 10 + 20.5")
  {
    const auto sum = std::make_shared<origami::ast::AstMathOperator>("+");
    sum->setLeftChild(std::make_shared<origami::ast::AstNumber>(10));
    sum->setRightChild(std::make_shared<origami::ast::AstNumber>(20.5));
    const std::any result = sum->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(result));
    REQUIRE(std::any_cast<double>(result) == 30.5);
  }
  SECTION("Несколько суммирований чисел: 10 + 20 + 30 + 15.5")
  {
    const auto first_part = std::make_shared<origami::ast::AstMathOperator>("+");
    first_part->setLeftChild(std::make_shared<origami::ast::AstNumber>(10));
    first_part->setRightChild(std::make_shared<origami::ast::AstNumber>(20));

    const auto second_part = std::make_shared<origami::ast::AstMathOperator>("+");
    second_part->setLeftChild(first_part);
    second_part->setRightChild(std::make_shared<origami::ast::AstNumber>(30));

    const auto third_part = std::make_shared<origami::ast::AstMathOperator>("+");
    third_part->setLeftChild(second_part);
    third_part->setRightChild(std::make_shared<origami::ast::AstNumber>(15.5));
    const std::any result = third_part->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(result));
    REQUIRE(std::any_cast<double>(result) == 75.5);
  }
  SECTION("Несколько суммирований чисел: 10 + 20 + 30 + 15.5")
  {
    const auto first_part = std::make_shared<origami::ast::AstMathOperator>("+");
    first_part->setLeftChild(std::make_shared<origami::ast::AstNumber>(10));
    first_part->setRightChild(std::make_shared<origami::ast::AstNumber>(20));

    const auto second_part = std::make_shared<origami::ast::AstMathOperator>("+");
    second_part->setLeftChild(std::make_shared<origami::ast::AstNumber>(30));
    second_part->setRightChild(std::make_shared<origami::ast::AstNumber>(15.5));
    const auto third_part = std::make_shared<origami::ast::AstMathOperator>("+");
    third_part->setLeftChild(first_part);
    third_part->setRightChild(second_part);
    const std::any result = third_part->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(result));
    REQUIRE(std::any_cast<double>(result) == 75.5);
  }
  SECTION("Создание абстрактного синтаксического дерева парсером для вычисление суммы целых чисел")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "2 + 3 + 10 + 15" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 30);
  }
  SECTION("Создание абстрактного синтаксического дерева парсером вычисление суммы целых и дробных чисел")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "2 + 3.3 + 10 + 15.5" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(double));
    REQUIRE_NOTHROW(std::any_cast<double>(result));
    REQUIRE(std::any_cast<double>(result) == 30.8);
  }
  SECTION("Создание абстрактного синтаксического дерева парсером вычисление выражения 7 + 10 - 2 + 16 - 5")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "7 + 10 - 2 + 16 - 5" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 26);
  }
  SECTION("Создание абстрактного синтаксического дерева парсером вычисление выражения (7 + 10) - 2 + 16 - 5 со скобками")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "(7 + 10) - (2 + 16 - 5)" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 4);
  }
  SECTION("Вычисление выражения 7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "7 + 3 * (10 / (12 / (3 + 1) - 1)) / (2 + 3) - 5 - 3 + (8)" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 10);
  }
  SECTION("Вычисление выражения 7 + (((3 + 2)))")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "7 + (((3 + 2)))" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 12);
  }
  SECTION("Проверка на работу с унарными операторами")
  {
    const auto ast = origami::parser::SyntaxAnalyzerCpp{ "5 - - - + - (3 + 4) - +2" }.parse();
    REQUIRE(ast);

    const std::any result = ast->accept(visitor);
    REQUIRE(result.has_value());
    REQUIRE(result.type() == typeid(int));
    REQUIRE_NOTHROW(std::any_cast<int>(result));
    REQUIRE(std::any_cast<int>(result) == 10);
  }
}