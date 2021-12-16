// ЛР6 Леухин М.В. М8О-206Б-20
// Вариант 4 - спроектировать и запрограммировать программу на языке C++
// создать шаблон динамической коллекции (стек), содержащий фигуры (трапеция)
// реализовать аллокатор (на очереди)

#include <iostream>
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