#ifndef OOP_EXERCISE_05_STACK_H
#define OOP_EXERCISE_05_STACK_H

#include <algorithm>
#include <memory>
#include <queue>
#include <cstdlib>

template<class T>
using vertex_t = std::pair<T, T>;

template<class T>
struct Trapezoid {
    vertex_t<T> vertex[4];
    Trapezoid(vertex_t<T> v1, vertex_t<T> v2, vertex_t<T> v3, vertex_t<T> v4);
    Trapezoid();
    double area() const;
    bool isValid() const;
    template<class U> friend std::ostream& operator<< (std::ostream& out, const Trapezoid<U>& e);
};

template<class T, size_t Counter>
struct Allocator {
    int size{};
    using Byte = char;
    T* used_blocks;
    std::queue<T*> free_blocks;

    Allocator();
    T* allocate();
    void deallocate(T* ptr);
    bool empty() const;
    int get_size() const;

    ~Allocator();
};


template<class T>
struct StackElement {
    std::shared_ptr<T> value;
    std::shared_ptr<StackElement<T>> next;
    std::shared_ptr<StackElement<T>> previous;
    StackElement(T value, std::shared_ptr<StackElement<T>> next,
                 std::shared_ptr<StackElement<T>> previous);
    StackElement();
    StackElement(StackElement<T>* s);
};

template<class T>
struct Stack;

template<class T>
struct Iterator {
    typedef std::random_access_iterator_tag iterator_category;
    typedef int value_type;
    typedef int difference_type;
    typedef int* pointer;
    typedef int& reference;
    std::shared_ptr<Stack<T>> stack_ptr;
    std::shared_ptr<StackElement<T>> current;
    explicit Iterator(std::shared_ptr<Stack<T>> s);
    Iterator(std::shared_ptr<Stack<T>> s, std::shared_ptr<StackElement<T>> ptr);
    Iterator<T>& operator++ ();
    Iterator<T>& operator-- ();
    bool operator== (Iterator<T> it);
    bool operator != (Iterator<T> it);
    T& operator* ();
};

template<class T>
struct Stack {
    Allocator<StackElement<T>, 512> allocator;
    std::shared_ptr<Stack<T>> self;
    std::shared_ptr<StackElement<T>> first;
    std::shared_ptr<StackElement<T>> last;
    int size;
    Stack();
    std::shared_ptr<T> pop();
    void push(T element);
    std::shared_ptr<T> top() const;
    Iterator<T> begin() const;
    Iterator<T> end() const;
    void insert(T elem, int i);
    void erase(int i);
    void print(int i) const;
    void print() const;
    int count(double i) const;
};

#endif //OOP_EXERCISE_05_STACK_H
