#include <clang/Tooling/Tooling.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <llvm/Support/Casting.h>

class AstVisitor final : public clang::RecursiveASTVisitor<AstVisitor>
{
  using BaseVisitor = clang::RecursiveASTVisitor<AstVisitor>;

public:
  bool TraverseDecl(clang::Decl* t_declaration);
};

bool AstVisitor::TraverseDecl(clang::Decl* t_declaration)
{
  if (t_declaration == nullptr) {
    return BaseVisitor::TraverseDecl(t_declaration);
  }

  if (const auto* functionDecl = llvm::dyn_cast<clang::FunctionDecl>(t_declaration); functionDecl) {
  }

  return true;
}

int main(int argc, char** argv)
{
  const char* code = R"code(
    int main() {
      return 0;
    }
  )code";

  const std::unique_ptr<clang::ASTUnit> ast = clang::tooling::buildASTFromCodeWithArgs(code, { "-std=c++1z" });
  assert(ast && "AST wasn't created");

  clang::TranslationUnitDecl* translationUnit{ ast->getASTContext().getTranslationUnitDecl() };
  assert(AstVisitor{}.TraverseDecl(translationUnit));

  return 0;
}