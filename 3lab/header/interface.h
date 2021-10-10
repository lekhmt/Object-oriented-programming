// Copyright 2021 lekhmt

#ifndef OOP_EXERCISE_02_INTERFACE_H
#define OOP_EXERCISE_02_INTERFACE_H

#include "figure.h"

class Interface {
public:
    std::vector<Figure*> memory;
    void add_octagon(coordinates_t points);
    void add_triangle(coordinates_t points);
    void add_square(coordinates_t points);
    void print_figure(int i) const;
    void print_area(int i) const;
    void print_center(int i) const;
    void remove_figure(int i) const;
    void print_all() const;
    void areas_all() const;
    void centers_all() const;
    void summary_area() const;
};


#endif //OOP_EXERCISE_02_INTERFACE_H
