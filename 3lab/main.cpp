// ЛР3 Леухин М.В. М8О-206Б-20
// Вариант 32 - разработать классы 8-угольника, треугольника и квадрата, наследуемые от базового класса Figure.
// Классы должны содержать набор следующих методов: вычисление геометрического центра фигуры, вывод в стандартный
// поток вывода std::cout координаты вершин фигуры, вычисление площади фигуры.

#include "Scanner.h"
#include "Parser.h"

int main(int argc, char* argv[]) {
    if (argc == 2) {
        wchar_t* file = coco_string_create(argv[1]);
        auto* scanner = new Scanner(file);
        auto* parser  = new Parser(scanner);
        parser->Parse();
        delete parser;
        delete scanner;
        delete file;
        return 0;
    } else {
        std::cout << "Use: translator filename\n";
        return 1;
    }
}