// Copyright 2021 lekhmt
// ЛР2 Леухин М.В. М8О-206Б-20
// Вариант 11 - класс Vector, задаваемый тройкой координат.
// Реализовать операции сложения и вычитания векторов, скалярное
// и векторное произведение векторов, умножение на скаляр,
// сравнение векторов на совпадение, вычисление длины вектора,
// сравнение длины векторов, вычисление угла между векторами.

#include "Parser.h"
#include "Scanner.h"
#include <vector>
#include <iostream>

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
