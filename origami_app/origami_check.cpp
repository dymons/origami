#include <clang/Tooling/Tooling.h>

int main(int argc, char** argv)
{
  const auto ast = clang::tooling::buildASTFromCodeWithArgs({ "int main() {return 0;}" }, { "-std=c++1z" });

  return 0;
}