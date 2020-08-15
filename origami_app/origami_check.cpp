#include <clang/Tooling/Tooling.h>
#include <clang/AST/RecursiveASTVisitor.h>

#include <llvm/Support/Casting.h>
#include <iostream>

class AstVisitor final : public clang::RecursiveASTVisitor<AstVisitor>
{
  using BaseVisitor = clang::RecursiveASTVisitor<AstVisitor>;

public:
  bool TraverseDecl(clang::Decl* t_declaration);
};

bool AstVisitor::TraverseDecl(clang::Decl* t_declaration)
{
  if (!t_declaration) {
    return BaseVisitor::TraverseDecl(t_declaration);
  }

  if (const auto* function = llvm::dyn_cast<clang::FunctionDecl>(t_declaration); function) {
    std::cout << "<FunctionDecl> ";

    // Определяем категорию, к которой относится функция.
    switch (function->getTemplatedKind()) {
      case clang::FunctionDecl::TK_NonTemplate:
        std::cout << "clang::FunctionDecl::TK_NonTemplate\n";
        break;
      case clang::FunctionDecl::TK_FunctionTemplate:
        std::cout << "clang::FunctionDecl::TK_FunctionTemplate\n";
        break;
      case clang::FunctionDecl::TK_MemberSpecialization:
        std::cout << "clang::FunctionDecl::TK_MemberSpecialization\n";
        break;
      case clang::FunctionDecl::TK_FunctionTemplateSpecialization:
        std::cout << "clang::FunctionDecl::TK_FunctionTemplateSpecialization\n";
        break;
      case clang::FunctionDecl::TK_DependentFunctionTemplateSpecialization:
        std::cout << "clang::FunctionDecl::TK_DependentFunctionTemplateSpecialization\n";
        break;
    }

    // Полечаем аттрибуты функции.
    if (function->hasAttrs()) {
      std::cout << " Attributes: ";
      std::for_each(function->attr_begin(), function->attr_end(), [](const auto& t_attr) { std::cout << t_attr->getSpelling() << " "; });
    }

    // Возвращаемое значение
    std::cout << " | Return type: " << function->getReturnType().getAsString();
    std::cout << " | Name function: " << function->getNameInfo().getAsString();

    // Параметры функции
    if (!function->param_empty()) {
      std::cout << " | Parametrs: ";
      for (const auto& param : function->parameters()) {
        std::cout << "[Type: " << param->getType().getAsString() << ", Name: " << param->getNameAsString() << "] ";
      }
    }

    // noexcept specification
    const auto* type = function->getType().getTypePtr();
    if (const auto* functionProto = llvm::dyn_cast<clang::FunctionProtoType>(type); functionProto) {
      if (clang::isNoexceptExceptionSpec(functionProto->getExceptionSpecType())) {
        std::cout << " | noexcept specification\n";
      }
    }
  }

  return BaseVisitor::TraverseDecl(t_declaration);
}

int main(int argc, char** argv)
{
  const char* code = R"code(
    [[nodiscard]] int main(int argc, char** argv) noexcept(true) {
      return 0;
    }
  )code";

  const std::unique_ptr<clang::ASTUnit> ast = clang::tooling::buildASTFromCodeWithArgs(code, { "-std=c++1z" });
  assert(ast && "AST wasn't created");

  clang::TranslationUnitDecl* translationUnit{ ast->getASTContext().getTranslationUnitDecl() };
  assert(AstVisitor{}.TraverseDecl(translationUnit));

  return 0;
}