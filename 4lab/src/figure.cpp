// Copyright 2021 lekhmt

#include "../header/figure.h"
#include <cmath>

Triangle::Triangle() {
    a = {0, 0}; b = {0, 0}; c = {0, 0};
}

Triangle::Triangle(point_t p0, point_t p1, point_t p2) {
    a = p0; b = p1; c = p2;
}

Triangle::Triangle(const Triangle& t) {
    a = t.a; b = t.b; c = t.c; is_removed = t.is_removed;
}

point_t Triangle::center() const {
    double x = (a.x + b.x + c.x) / 3;
    double y = (a.y + b.y + c.y) / 3;
    return {x, y};
}

double Triangle::area() const {
    return 0.5 * std::abs((b.x - a.x) * (c.y - a.y) -
                             (c.x - a.x) * (b.y - a.y));
}

std::wostream& operator<< (std::wostream& out, const Triangle& t){
    out << "(" << t.a.x << ", " << t.a.y << "), "
        << "(" << t.b.x << ", " << t.b.y << "), "
        << "(" << t.c.x << ", " << t.c.y << "); "
        << "area is " << t.area() << "\n";
    return out;
}

bool operator== (const Triangle& t1, const Triangle& t2){
    return t1.a.x == t2.a.x &&
           t1.a.y == t2.a.y &&
           t1.b.x == t2.b.x &&
           t1.b.y == t2.b.y &&
           t1.c.x == t2.c.x &&
           t1.c.y == t2.c.y;
}