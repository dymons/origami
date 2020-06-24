# origami

Оригами - вид декоративного прикладного искусства, позволяющая создавать объемные фигуры из бумаги. Оригами содержит под категорию
`Модульное оригами`, которое заключается в создании объемных фигур из нескольких листов бумаги. Создание статического анализатора берет
за основу понятие модульности, чтобы в зависимости от используемой библиотеки в проекте, можно было использование характерные правила
разработки, учитывающие особенности работы той или иной библиотеки.

## Building
```shell
https://github.com/dymons/origami
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE="Release" .. && cmake --build .
```

## TODO
1. Сделать поддержку для Python3. Написать тесты охватывающие весь спектр языка.
2. Написать тесты охватывающие весь спектр языка С++.
3. Добавить документацию, есть места где я пропустил.
4. Изучить идею абстрактного синтаксического дерева. Спроектировать его реализацию.
5. Добавить описание текущей архитектуры лексического анализатора.
6. Создание интерпретатора для С++ и компилятора для Python.
7. Лексическую конвенцию через xml?
8. Реализация AST через dynamic_cast и ID класса?