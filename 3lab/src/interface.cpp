// Copyright 2021 lekhmt

#include "../header/interface.h"
#include <iostream>

typedef point_t vector_t;

void Interface::add_octagon(coordinates_t points) {
    auto* t = new Octagon(points[0], points[1], points[2], points[3],
                        points[4], points[5], points[6], points[7]);
    memory.push_back(t);
}

void Interface::add_triangle(coordinates_t points) {
    auto* t = new Triangle(points[0], points[1], points[2]);
    memory.push_back(t);
}

void Interface::add_square(coordinates_t points) {
    auto* t = new Square(points[0], points[1], points[2], points[3]);
    vector_t v = { points[1].first - points[0].first, points[1].second - points[0].second };
    vector_t v1 = { points[2].first - points[1].first, points[2].second - points[1].second };
    vector_t v2 = { points[3].first - points[2].first, points[3].second - points[2].second };
    vector_t v3 = { points[0].first - points[3].first, points[0].second - points[3].second };
    double v_by_v1 = v.first * v1.first + v.second * v1.second;
    double v_by_v3 = v.first * v3.first + v.second * v3.second;
    memory.push_back(t);
}

void Interface::print_figure(int i) const {
    if (i < 0 || i >= memory.size()){
        std::wcout << "Error: index out of bounds\n";
        return;
    }
    Figure* t = memory[i];
    if (t->removed){
        std::wcout << "Error: figure " << i + 1 << " was removed\n";
        return;
    }
    if (t->data.size() == 8){ std::wcout << "> " << i + 1 << " octagon "; }
    if (t->data.size() == 3){ std::wcout << "> " << i + 1 << " triangle "; }
    if (t->data.size() == 4){ std::wcout << "> " << i + 1 << " square "; }
    for (auto e : t->data){
        std::wcout << "(" << e.first << ", " << e.second << ") ";
    }
    std::wcout << "\n";
}

void Interface::print_area(int i) const {
    if (i < 0 || i >= memory.size()){
        std::wcout << "Error: index out of bounds\n";
        return;
    }
    Figure* t = memory[i];
    if (t->removed){
        std::wcout << "Error: figure " << i + 1 << " was removed\n";
        return;
    }
    if (t->data.size() == 8){ std::wcout << "> " << i + 1 << " octagon area is "; }
    if (t->data.size() == 3){ std::wcout << "> " << i + 1 << " triangle area is "; }
    if (t->data.size() == 4){ std::wcout << "> " << i + 1 << " square area is "; }
    std::wcout << t->area() << "\n";
}

void Interface::print_center(int i) const {
    if (i < 0 || i >= memory.size()){
        std::wcout << "Error: index out of bounds\n";
        return;
    }
    Figure* t = memory[i];
    if (t->removed){
        std::wcout << "Error: figure " << i + 1 << " was removed\n";
        return;
    }
    if (t->data.size() == 8){ std::wcout << "> " << i + 1 << " octagon center is "; }
    if (t->data.size() == 3){ std::wcout << "> " << i + 1 << " triangle center is "; }
    if (t->data.size() == 4){ std::wcout << "> " << i + 1 << " square center is "; }
    point_t center = t->center();
    std::wcout << "(" << center.first << ", " << center.second << ")\n";
}

void Interface::remove_figure(int i) const {
    if (i < 0 || i >= memory.size()){
        std::wcout << "Error: index out of bounds\n";
        return;
    }
    memory[i]->removed = true;
    std::wcout << "> figure " << i + 1 << " was removed\n";
}

void Interface::print_all() const {
    for (int i = 0; i < memory.size(); ++i){
        if (!memory[i]->removed){
            print_figure(i);
        }
    }
}

void Interface::areas_all() const {
    for (int i = 0; i < memory.size(); ++i){
        if (!memory[i]->removed){
            print_area(i);
        }
    }
}

void Interface::centers_all() const {
    for (int i = 0; i < memory.size(); ++i){
        if (!memory[i]->removed){
            print_center(i);
        }
    }
}

void Interface::summary_area() const {
    double s = 0;
    for (auto e : memory){
        if (!e->removed){
            s += e->area();
        }
    }
    std::wcout << "> summary area is " << s << "\n";
}