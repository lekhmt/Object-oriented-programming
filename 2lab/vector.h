// Copyright 2021 lekhmt

#ifndef OOP_EXERCISE_02_VECTOR_H
#define OOP_EXERCISE_02_VECTOR_H

#include <map>

class Vector {
 public:
    Vector(): vx(0), vy(0), vz(0), vlength(0) {}

    Vector(double x, double y, double z);

    Vector(const Vector& v);

    double x() const;
    double y() const;
    double z() const;
    double length() const;

    bool is_match(const Vector& v) const;

    bool is_equal(const Vector& v) const;

    friend std::istream& operator>> (std::istream& in, Vector& v);

 private:
    double vx, vy, vz;
    double vlength;
};

class Memory {
 public:
    void save(std::wstring name, const Vector& v);
    Vector get(std::wstring name);

 private:
    std::map<std::wstring, Vector> memory;
};

std::wostream& operator<< (std::wostream& out, const Vector& v);
Vector operator+ (const Vector& v1, const Vector& v2);
Vector operator- (const Vector& v1, const Vector& v2);
double operator* (const Vector& v1, const Vector& v2);
Vector operator* (double s, const Vector& v);
Vector operator* (const Vector& v, double s);
Vector operator& (const Vector& v1, const Vector& v2);

double cos_angle(const Vector& v1, const Vector& v2);
double angle(const Vector& v1, const Vector& v2);

#endif  // OOP_EXERCISE_02_VECTOR_H
