// Copyright 2021 lekhmt

#ifndef OOP_EXERCISE_03_INTERFACE_H
#define OOP_EXERCISE_03_INTERFACE_H

#include "figure.h"

class Vector {
public:
    int size;
    Triangle* buffer;

    Vector();
    void append(const Triangle& t);
    void set(const Triangle& t, int i) const;
    Triangle get(int i) const;
    void remove(int i) const;

    friend std::wostream& operator<< (std::wostream& out, const Vector& v);

    ~Vector();

};

#endif //OOP_EXERCISE_03_INTERFACE_H
