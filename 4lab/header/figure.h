// Copyright 2021 lekhmt

#ifndef OOP_EXERCISE_02_FIGURE_H
#define OOP_EXERCISE_02_FIGURE_H

#include <iostream>

struct point_t {
    double x, y;
};

typedef point_t coordinates_t[3];

class Triangle {
public:
    bool is_removed = false;
    point_t a{}, b{}, c{};
    Triangle();
    Triangle(point_t p0, point_t p1, point_t p2);
    Triangle(const Triangle& t);
    point_t center() const;
    double area() const;

    friend std::wostream& operator<< (std::wostream& out, const Triangle& t);
    friend bool operator== (const Triangle& t1, const Triangle& t2);

};

#endif //OOP_EXERCISE_02_FIGURE_H
