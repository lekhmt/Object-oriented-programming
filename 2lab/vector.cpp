// Copyright 2021 lekhmt

#include "vector.h"
#include <string>
#include <iostream>
#include <cmath>

bool is_number(const std::string& s) {
    bool point = false;
    for (int i = 0; i < s.length(); ++i) {
        if (s[i] == '-' && i == 0) {
            continue;
        } else if (s[i] == '.') {
            if ((i == 0 || i == s.length() - 1) || point) {
                return false;
            } else {
                point = true;
            }
        } else if (s[i] < '0' || s[i] > '9') { return false; }
    }
    return true;
}

Vector::Vector(double x, double y, double z): vx(x), vy(y), vz(z) {
    vlength = sqrt(x * x + y * y + z * z);
}

Vector::Vector(const Vector& v):
    vx(v.x()), vy(v.y()), vz(v.z()), vlength(v.length()) {}

double Vector::x() const { return vx; }
double Vector::y() const { return vy; }
double Vector::z() const { return vz; }
double Vector::length() const { return vlength; }

bool Vector::is_match(const Vector& v) const {
    return vx == v.x() && vy == v.y() && vz == v.z();
}

bool Vector::is_equal(const Vector& v) const {
    return vlength == v.length();
}

std::istream& operator>> (std::istream& in, Vector& v) {
    std::string x, y, z; std::cin >> x >> y >> z;

    /*if (!is_number(x) || !is_number(y) || !is_number(z)){
        std::cout << "Ошибка! Некорректный ввод.\n";
        exit(1);
    }*/

    // std::cout << "Введено: " << x << "\n";

    v.vx = stod(x); v.vy = stod(y); v.vz = stod(z);
    v.vlength = sqrt(v.x() * v.x() + v.y() * v.y() + v.z() * v.z());
    return in;
}

std::wostream& operator<< (std::wostream& out, const Vector& v) {
    std::wcout << "(" << v.x() << ", " << v.y() << ", " << v.z()
               << ")\n";
    return out;
}

Vector operator+ (const Vector& v1, const Vector& v2) {
    return {v1.x() + v2.x(), v1.y() + v2.y(), v1.z() + v2.z()};
}

Vector operator- (const Vector& v1, const Vector& v2) {
    return {v1.x() - v2.x(), v1.y() - v2.y(), v1.z() - v2.z()};
}

// скалярное произведение векторов
double operator* (const Vector& v1, const Vector& v2) {
    return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
}

// произведение веткора на скаляр
Vector operator* (const double s, const Vector& v) {
    return {v.x() * s, v.y() * s, v.z() * s};
}

Vector operator* (const Vector& v, const double s) {
    return s * v;
}

// векторное произведение векторов
Vector operator& (const Vector& v1, const Vector& v2) {
    return {v1.y() * v2.z() - v1.z() * v2.y(),
            v1.z() * v2.x() - v1.x() * v2.z(),
            v1.x() * v2.y() - v1.y() * v2.x()};
}

// вычисление косинуса угла между векторами
double cos_angle(const Vector& v1, const Vector& v2) {
    return (v1 * v2)/(v1.length() * v2.length());
}

// вычисление угла между вектоарми
double angle(const Vector& v1, const Vector& v2) {
    double ca = cos_angle(v1, v2);
    if (ca >= 1 && round(ca) == 1) { return 0; }
    if (ca <= -1 && round(ca) == -1) { return acos(-1); }
    return acos(cos_angle(v1, v2));
}

void Memory::save(std::wstring name, const Vector& v) {
    memory[name] = v;
}

Vector Memory::get(std::wstring name) {
    return memory[name];
}
