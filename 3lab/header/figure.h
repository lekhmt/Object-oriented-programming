// Copyright 2021 lekhmt

#ifndef OOP_EXERCISE_02_FIGURE_H
#define OOP_EXERCISE_02_FIGURE_H

#include <vector>

typedef std::pair<double, double> point_t;
typedef std::vector<point_t> coordinates_t;


class Figure {
public:
    bool removed = false;
    coordinates_t data;
    virtual point_t center() { return {0, 0}; };
    virtual double area() { return 0; };
};

class Octagon: public Figure {
public:
    Octagon(point_t p0, point_t p1, point_t p2, point_t p3,
            point_t p4, point_t p5, point_t p6, point_t p7);
    point_t center() override;
    double area() override;
};

class Triangle: public Figure {
public:
    Triangle(point_t p0, point_t p1, point_t p2);
    point_t center() override;
    double area() override;
};

class Square: public Figure {
public:
    Square(point_t p0, point_t p1, point_t p2, point_t p3);
    point_t center() override;
    double area() override;
};

#endif //OOP_EXERCISE_02_FIGURE_H
