#include <algorithm>
#include <memory>
#include <iostream>
#include "stack.h"
#include <cmath>

template struct Stack<Trapezoid<int>>;
template struct Trapezoid<int>;
template struct Allocator<StackElement<Trapezoid<int>>, 512>;

// Trapezoid

template<class T>
Trapezoid<T>::Trapezoid(vertex_t<T> v1, vertex_t<T> v2, vertex_t<T> v3, vertex_t<T> v4) {
    vertex[0] = v1;
    vertex[1] = v2;
    vertex[2] = v3;
    vertex[3] = v4;
}

template<class T>
Trapezoid<T>::Trapezoid(){
    vertex[0] = {0, 0};
    vertex[1] = {0, 0};
    vertex[2] = {0, 0};
    vertex[3] = {0, 0};
}

template<class T>
double Trapezoid<T>::area() const{
    return 0.5 * std::abs((vertex[1].first - vertex[0].first) * (vertex[2].second - vertex[0].second) -
                          (vertex[2].first - vertex[0].first) * (vertex[1].second - vertex[0].second)) +
           0.5 * std::abs((vertex[2].first - vertex[0].first) * (vertex[3].second - vertex[0].second) -
                          (vertex[3].first - vertex[0].first) * (vertex[2].second - vertex[0].second));
}

struct Point {
    int x, y;
    double dist(Point other) const {
        double dx = (other.x - x);
        double dy = (other.y - y);
        return std::sqrt(dx*dx + dy*dy);
    }
};

template<class T>
bool Trapezoid<T>::isValid() const {
    Point a = {vertex[0].first, vertex[0].second};
    Point b = {vertex[1].first, vertex[1].second};
    Point c = {vertex[2].first, vertex[2].second};
    Point d = {vertex[3].first, vertex[3].second};
    Point v1 = {b.x - a.x, b.y - a.y};
    Point v2 = {c.x - b.x, c.y - b.y};
    Point v3 = {d.x - c.x, d.y - c.y};
    Point v4 = {a.x - d.x, a.y - d.y};

    struct two_vectors {
        Point v1, v2;
        bool is_collinear = false;
    };

    two_vectors duet1 {v1, v3}, duet2 {v2, v4};

    duet1.is_collinear = (v1.x == 0 && v3.x ==0) || (v1.y == 0 && v3.y == 0) || ((v1.x / v3.x) == (v1.y / v3.y));
    duet2.is_collinear = (v2.x == 0 && v4.x ==0) || (v2.y == 0 && v4.y == 0) || ((v2.x / v4.x) == (v2.y / v4.y));

    double ab = a.dist(b);
    double bc = b.dist(c);
    double cd = c.dist(d);
    double da = d.dist(a);

    if (duet1.is_collinear || duet2.is_collinear) {
        double p1 = (ab + bc + a.dist(c)) / 2;
        double p2 = (da + cd + a.dist(c)) / 2;

        double sqr = sqrt(p1 * (p1 - ab) * (p1 - bc) * (p1 - a.dist(c)))
              + sqrt(p2 * (p2 - da) * (p2 - cd) * (p2 - a.dist(c)));

        if (sqr > 0)
            return true;

        return false;
    }

    return false;
}

template<class U>
std::ostream& operator<< (std::ostream& out, const Trapezoid<U>& e){
    std::cout << "trapezoid (" << e.vertex[0].first << ", " << e.vertex[0].second << "), " <<
              "(" << e.vertex[1].first << ", " << e.vertex[1].second << "), " <<
              "(" << e.vertex[2].first << ", " << e.vertex[2].second << "), " <<
              "(" << e.vertex[3].first << ", " << e.vertex[3].second << "); area is " << e.area();
    return out;
}

// Allocator

template<class T, size_t Counter>
Allocator<T, Counter>::Allocator() {
    this->size = Counter;
    used_blocks = (T*) malloc(sizeof(T) * Counter);
    for (int i = 0; i < Counter; ++i){
        T* ptr = used_blocks + i * sizeof(T);
        free_blocks.push(ptr);
    }
}

template<class T, size_t Counter>
T* Allocator<T, Counter>::allocate() {
    if (!free_blocks.empty()){
        T* res = free_blocks.front();
        free_blocks.pop();
        return res;
    } else {
        throw std::bad_alloc();
    }
}

template<class T, size_t Counter>
void Allocator<T, Counter>::deallocate(T* ptr) {
    free_blocks.push(ptr);
}

template<class T, size_t Counter>
bool Allocator<T, Counter>::empty() const {
    return free_blocks.empty();
}

template<class T, size_t Counter>
int Allocator<T, Counter>::get_size() const {
    return size;
}

template<class T, size_t Counter>
Allocator<T, Counter>::~Allocator() {
    while (!free_blocks.empty()){
        free_blocks.pop();
    }
}

// StackElement

template<class T>
StackElement<T>::StackElement(T value, std::shared_ptr<StackElement<T>> next,
                              std::shared_ptr<StackElement<T>> previous) {
    this->value = std::make_shared<T>(value);
    this->next = next;
    this->previous = previous;
}

template<class T>
StackElement<T>::StackElement(){
    this->value = std::make_shared<T>();
    this->next = nullptr;
    this->previous = nullptr;
}

template<class T>
StackElement<T>::StackElement(StackElement<T>* s){
    this->value = s->value;
    this->next = s->next;
    this->previous = s->previous;
}

// Iterator

template<class T>
Iterator<T>::Iterator(std::shared_ptr<Stack<T>> s){
    stack_ptr = s;
    current = s->first;
}

template<class T>
Iterator<T>::Iterator(std::shared_ptr<Stack<T>> s, std::shared_ptr<StackElement<T>> ptr){
    stack_ptr = s;
    current = ptr;
}

template<class T>
Iterator<T>& Iterator<T>::operator++ (){
    current = current->next;
}

template<class T>
Iterator<T>& Iterator<T>::operator-- (){
    current = current->previous;
}

template<class T>
bool Iterator<T>::operator== (const Iterator<T> it){
    return this->stack_ptr == it.stack_ptr && this->current == it.current;
}

template<class T>
bool Iterator<T>::operator != (const Iterator<T> it){
    return !(*this == it);
}

template<class T>
T& Iterator<T>::operator* (){
    return *(this->current->value);
}

// Stack

template<class T>
Stack<T>::Stack(){
    self = std::make_shared<Stack<T>>(*this);
    auto tmp = std::make_shared<StackElement<T>>();
    first = last = tmp;
    size = 0;
}

template<class T>
std::shared_ptr<T> Stack<T>::pop(){
    if (!size){
        std::wcout << "error: stack is empty\n";
        return nullptr;
    }
    auto tmp = last->previous;
    if (size == 1){
        first = last;
        size = 0;
    } else {
        last->previous->previous->next = last;
        last->previous = last->previous->previous;
        --size;
    }
    return tmp->value;
}

template<class T>
void Stack<T>::push(T element) {
    if (!element.isValid()){
        std::cout << "error: invalid figure " << element << "\n";
        return;
    }
    auto stmp = new (allocator.allocate()) StackElement<T>(element, last, last->previous);
    std::shared_ptr<StackElement<T>> tmp = std::make_shared<StackElement<T>>(stmp);
    if (!size){
        first = tmp;
        first->next = last;
        last->previous = first;
        size = 1;
    } else {
        last->previous->next = tmp;
        last->previous = tmp;
        ++size;
    }
}

template<class T>
std::shared_ptr<T> Stack<T>::top() const{
    if (!size){
        std::wcout << "error: stack is empty\n";
        return nullptr;
    }
    return last->previous->value;
}

template<class T>
Iterator<T> Stack<T>::begin() const{
    return Iterator<T>(self, first);
}

template<class T>
Iterator<T> Stack<T>::end() const{
    return Iterator<T>(self, last);
}

template<class T>
void Stack<T>::insert(T elem, int i){
    if (i < -1 || i > this->size - 1){
        std::cout << "error: invalid position\n";
        return;
    }
    auto it = this->begin(); for (int k = 0; k < i; ++k){ ++it; }
    auto tmp = std::make_shared<StackElement<T>>(elem, it.current->next, it.current);
    if (i == -1){
        tmp->previous = nullptr;
        tmp->next = this->first;
        this->first = tmp;
    } else {
        it.current->next->previous = tmp;
        it.current->next = tmp;
    }
}

template<class T>
void Stack<T>::erase(int i){
    if (i < 0 || i > this->size - 1){
        std::cout << "error: invalid position\n";
        return;
    }
    auto it = this->begin(); for (int k = 0; k < i; ++k){ ++it; }
    if (i == 0){
        this->first = this->first->next;
        this->first->previous = nullptr;
        return;
    }
    it.current->previous->next = it.current->next;
    it.current->next->previous = it.current->previous;
}

template<class T>
void Stack<T>::print(int i) const{
    auto it = this->begin();
    for (int k = 0; k < i; ++k){ ++it; }
    std::cout << *(it.current->value) << std::endl;
}

template<class T>
void Stack<T>::print() const{
    std::for_each(this->begin(), this->end(), [](T i){ std::cout << i << "\n"; });
}

template<class T>
double get_area(Trapezoid<int> i, Stack<T> s){
    return i.area();
}

template<class T>
int Stack<T>::count(double a) const{
    return std::count_if(this->begin(), this->end(), [&a](T i){ return i.area() < a; });
}



