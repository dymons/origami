## origami_check

Разработка статического анализатора на основе использования открытого API CLang для создания абстрактного синтаксического дерева и последующего обхода.

```
cmake .. -G "Unix Makefiles" -DORIGAMI_LLVM_ROOT_PATH=/usr/lib/llvm-6.0
cmake -build .
```

## TODO
1. https://clang.llvm.org/extra/clang-tidy/
2. https://clang.llvm.org/docs/LibASTMatchersReference.html
3. https://clang.llvm.org/docs/IntroductionToTheClangAST.html