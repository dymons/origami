#include "origami_lexical/tokenizers/lexical_conventions.hpp"

#include <vector>

#include <gtest/gtest.h>

class PreprocessorsTest : public ::testing::Test {
  protected:
    origami::lex::LexicalConventions m_tokenizer;
};

TEST_F(PreprocessorsTest, IncludeTest)
{
  { // Проверка, на определение простого символа '#'
    const auto tokens = m_tokenizer.getTokens("#");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = m_tokenizer.getTokens("#include \"memory\"");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Literal, "\"memory\"" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST_F(PreprocessorsTest, DISABLE_Include)
{
  { // Проверка, на определение простого символа '#'
    const auto tokens = m_tokenizer.getTokens("#");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = m_tokenizer.getTokens("#     include      <memory>      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "<memory>" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате "header-name"
    const auto tokens = m_tokenizer.getTokens("#     include      \"memory\"      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "\"memory\"" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = m_tokenizer.getTokens("#  +   include      \"memory\"      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Identifier, "include" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = m_tokenizer.getTokens("#     include    +    \"memory\"      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = m_tokenizer.getTokens("#     include   int    \"memory\"      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение некорректного заголовочного файла
    const auto tokens = m_tokenizer.getTokens("#     include   0.0123    \"memory\"      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Literal, "0.0123" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, "\"" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение нескольких заголовочных файлов
    const auto tokens = m_tokenizer.getTokens("#     include      <memory>\n  #     include      \"algorithm\"    ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "<memory>" },
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "\"algorithm\"" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Простая программа с подключением двух заголовочных файлов
    const auto tokens = m_tokenizer.getTokens("#include <memory>\n#     include      \"origami_lexical/conventions/tokens.hpp\"\n  "
                                              "int main()\n{\n\treturn 0;\n}");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "<memory>" },
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "\"origami_lexical/conventions/tokens.hpp\"" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Identifier, "main" },
        { origami::lex::Token::Punctuator, "(" },
        { origami::lex::Token::Punctuator, ")" },
        { origami::lex::Token::Punctuator, "{" },
        { origami::lex::Token::Keyword, "return" },
        { origami::lex::Token::Literal, "0" },
        { origami::lex::Token::Punctuator, ";" },
        { origami::lex::Token::Punctuator, "}" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     icustomdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Identifier, "icustomdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#icustomdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Identifier, "icustomdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     includecustomdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Identifier, "includecustomdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#includecustomdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Identifier, "includecustomdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#include customdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Identifier, "customdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#include \"customdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Punctuator, "\"" },
        { origami::lex::Token::Identifier, "customdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     include");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#++     include");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Operator, "+" },
        { origami::lex::Token::Identifier, "include" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     include             ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     include      \n       ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     include      int int      ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Keyword, "int" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     include      int int   <memory>   ");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::KeywordPreprocessor, "#include" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Punctuator, "<" },
        { origami::lex::Token::Identifier, "memory" },
        { origami::lex::Token::Punctuator, ">" },
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST_F(PreprocessorsTest, DISABLED_Ifdef)
{
  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = m_tokenizer.getTokens("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXY");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXY" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }

  { // Проверка, на определение подключение заголовочного файла в формате <header-name>
    const auto tokens = m_tokenizer.getTokens("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz   \n"
                                              "int main()\n{\n\treturn 0;\n}");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Identifier, "main" },
        { origami::lex::Token::Punctuator, "(" },
        { origami::lex::Token::Punctuator, ")" },
        { origami::lex::Token::Punctuator, "{" },
        { origami::lex::Token::Keyword, "return" },
        { origami::lex::Token::Literal, "0" },
        { origami::lex::Token::Punctuator, ";" },
        { origami::lex::Token::Punctuator, "}" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST_F(PreprocessorsTest, DISABLED_Combination)
{
  { // Проверка, на подключение нескольких предпроцессоров последовательно
    const auto tokens = m_tokenizer.getTokens("#     ifdef      _0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz   \n"
                                              "#       define  NUM_SIZE  0.0\n"
                                              "#       endif \n"
                                              "#   ifndef ___0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz"
                                              "#define MCVC_USE\n"
                                              "#       endif \n"
                                              "int main()\n{\n\treturn 0;\n}");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Keyword, "#ifdef" },
        { origami::lex::Token::Identifier, "_0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
        { origami::lex::Token::Keyword, "#define" },
        { origami::lex::Token::Identifier, "NUM_SIZE" },
        { origami::lex::Token::Literal, "0.0" },
        { origami::lex::Token::Keyword, "#endif" },
        { origami::lex::Token::Keyword, "#ifndef" },
        { origami::lex::Token::Identifier, "___0123456789_ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz" },
        { origami::lex::Token::Keyword, "#define" },
        { origami::lex::Token::Identifier, "MCVC_USE" },
        { origami::lex::Token::Keyword, "#endif" },
        { origami::lex::Token::Keyword, "int" },
        { origami::lex::Token::Identifier, "main" },
        { origami::lex::Token::Punctuator, "(" },
        { origami::lex::Token::Punctuator, ")" },
        { origami::lex::Token::Punctuator, "{" },
        { origami::lex::Token::Keyword, "return" },
        { origami::lex::Token::Literal, "0" },
        { origami::lex::Token::Punctuator, ";" },
        { origami::lex::Token::Punctuator, "}" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}

TEST_F(PreprocessorsTest, DISABLED_Nonexistent)
{
  { // Проверка, на подключение не существующих предпроцессоров
    const auto tokens = m_tokenizer.getTokens("#     customdefine");
    const std::vector<std::pair<origami::lex::Token, std::string>> expect_tokens {
      {
        { origami::lex::Token::Punctuator, "#" },
        { origami::lex::Token::Identifier, "customdefine" }
      }
    };

    ASSERT_TRUE(std::equal(tokens.begin(), tokens.end(), expect_tokens.begin(), [](const auto& t_lhs, const auto& t_rhs) {
      return (t_lhs.first == t_rhs.first) && (t_lhs.second == t_rhs.second);
    }));
  }
}