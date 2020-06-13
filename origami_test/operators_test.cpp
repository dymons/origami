#include "origami_lexical/tokenizers/lexical_conventions.hpp"
#include "origami_lexical/symbol_table/symbol_table_cpp.hpp"

#include <catch2/catch.hpp>

TEST_CASE("Проверка разбиения программы С++ на токены", "[tokenizer-cpp]")
{
  origami::lex::LexicalConventions<origami::lex::SymbolTableCpp> tokenizer;
}