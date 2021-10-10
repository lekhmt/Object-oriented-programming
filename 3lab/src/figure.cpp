// Copyright 2021 lekhmt

#include "../header/figure.h"
#include <cmath>

Octagon::Octagon(point_t p0, point_t p1, point_t p2, point_t p3,
                 point_t p4, point_t p5, point_t p6, point_t p7) {
    data.resize(8);
    data[0] = p0; data[1] = p1; data[2] = p2; data[3] = p3;
    data[4] = p4; data[5] = p5; data[6] = p6; data[7] = p7;
}

point_t Octagon::center() {
    double x = 0, y = 0;
    for (auto e : data){
        x += e.first;
        y += e.second;
    }
    return {x / 8, y / 8};
}

double Octagon::area() {
    double s = 0;
    auto n = data.size();
    for (int i = 0; i < n - 1; ++i){
        s += data[i].first * data[i + 1].second;
    }
    s += data[n - 1].first * data[0].second;
    for (int i = 1; i < n; ++i){
        s -= data[i].first * data[i - 1].second;
    }
    s -= data[0].first * data[n - 1].second;
    return 0.5 * std::abs(s);
}


Triangle::Triangle(point_t p0, point_t p1, point_t p2) {
    data.resize(3);
    data[0] = p0;
    data[1] = p1;
    data[2] = p2;
}

point_t Triangle::center() {
    double x = (data[0].first + data[1].first + data[2].first) / 3;
    double y = (data[0].second + data[1].second + data[2].second) / 3;
    return {x, y};
}

double Triangle::area() {
    return 0.5 * std::abs((data[1].first - data[0].first) * (data[2].second - data[0].second) -
                             (data[2].first - data[0].first) * (data[1].second - data[0].second));
}


Square::Square(point_t p0, point_t p1, point_t p2, point_t p3) {
    data.resize(4);
    data[0] = p0;
    data[1] = p1;
    data[2] = p2;
    data[3] = p3;
}

point_t Square::center() {
    double x = (data[0].first + data[1].first + data[2].first + data[3].first) / 4;
    double y = (data[0].second + data[1].second + data[2].second + data[3].second) / 4;
    return {x, y};
}

double Square::area() {
    return pow(data[1].first - data[0].first, 2) + pow(data[1].second - data[0].second , 2);
}