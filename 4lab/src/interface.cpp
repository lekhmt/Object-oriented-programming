// Copyright 2021 lekhmt

#include "../header/interface.h"

Vector::Vector(){
    size = 0;
    buffer = nullptr;
}

void Vector::append(const Triangle &t) {
    auto tmp = new Triangle[size + 1];
    for (int i = 0; i < size; ++i){
        tmp[i] = buffer[i];
    }
    delete[] buffer;
    buffer = tmp;
    buffer[size] = t;
    if (t.area() == 0){
        std::wcout << "> error: " << size + 1 << " is not a triangle; automatically removed\n";
        buffer[size].is_removed = true;
    }
    ++size;
}

void Vector::set(const Triangle &t, int i) const {
    if (i < 0 || i >= size){
        std::wcout << "> error: index " << i + 1 << " out of bounds\n";
        return;
    }
    buffer[i] = t;
    if (t.area() == 0){
        std::wcout << "> error: " << i + 1 << " is not a triangle; automatically removed\n";
        buffer[i].is_removed = true;
    }
}

Triangle Vector::get(int i) const {
    if (i < 0 || i >= size){
        std::wcout << "> error: index " << i + 1 << " out of bounds\n";
        return {};
    }
    if (buffer[i].is_removed){
        std::wcout << "> error: " << i + 1 << " triangle is removed\n";
    }
    return buffer[i];
}

void Vector::remove(int i) const {
    if (i < 0 || i >= size){
        std::wcout << "> error: index " << i + 1 << " out of bounds\n";
        return;
    }
    buffer[i].is_removed = true;
    std::wcout << "> triangle " << i + 1 << " was removed\n";
}

std::wostream& operator<< (std::wostream& out, const Vector& v){
    for (int i = 0; i < v.size; ++i){
        if (!v.buffer[i].is_removed){
            out << i + 1 << ": " << v.buffer[i];
        }
    }
    return out;
}

Vector::~Vector(){
    delete[] buffer;
}