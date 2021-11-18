#ifndef OOP_EXERCISE_05_STACK_H
#define OOP_EXERCISE_05_STACK_H

#include <algorithm>
#include <memory>

template<typename T>
struct Triangle {
    using vertex_t = std::pair<T, T>;
    vertex_t vertex[3];
    Triangle(vertex_t v1, vertex_t v2, vertex_t v3) {
        vertex[0] = v1;
        vertex[1] = v2;
        vertex[2] = v3;
    }
    Triangle(){
        vertex[0] = {0, 0};
        vertex[1] = {0, 0};
        vertex[2] = {0, 0};
    }
    double area(){
        return 0.5 * std::abs((vertex[1].first - vertex[0].first) * (vertex[2].second - vertex[0].second) -
                              (vertex[2].first - vertex[0].first) * (vertex[1].second - vertex[0].second));
    }
    template<class U> friend std::ostream& operator<< (std::ostream& out, const Triangle<U>& e);
};

template<class U>
std::ostream& operator<< (std::ostream& out, const Triangle<U>& e){
    std::cout << "triangle (" << e.vertex[0].first << ", " << e.vertex[0].second << "), " <<
              "(" << e.vertex[1].first << ", " << e.vertex[1].second << "), " <<
              "(" << e.vertex[2].first << ", " << e.vertex[2].second << ")";
    return out;
}

template<class T>
struct StackElement {
    std::shared_ptr<T> value;
    std::shared_ptr<StackElement<T>> next;
    std::shared_ptr<StackElement<T>> previous;
    StackElement(T value, std::shared_ptr<StackElement<T>> next,
                 std::shared_ptr<StackElement<T>> previous) {
        this->value = std::make_shared<T>(value);
        this->next = next;
        this->previous = previous;
    }
    StackElement(){
        this->value = std::make_shared<T>();
        this->next = nullptr;
        this->previous = nullptr;
    }
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
    explicit Iterator(std::shared_ptr<Stack<T>> s){
        stack_ptr = s;
        current = s->first;
    }
    Iterator(std::shared_ptr<Stack<T>> s, std::shared_ptr<StackElement<T>> ptr){
        stack_ptr = s;
        current = ptr;
    }
    Iterator<T>& operator++ (){
        current = current->next;
    }
    Iterator<T>& operator-- (){
        current = current->previous;
    }
    bool operator== (const Iterator<T> it){
        return this->stack_ptr == it.stack_ptr && this->current == it.current;
    }
    bool operator != (const Iterator<T> it){
        return !(*this == it);
    }
    T& operator* (){
        return *(this->current->value);
    }
};

template<class T>
struct Stack {
    std::shared_ptr<Stack<T>> self;
    std::shared_ptr<StackElement<T>> first;
    std::shared_ptr<StackElement<T>> last;
    int size;
    Stack(){
        self = std::make_shared<Stack<T>>(*this);
        auto tmp = std::make_shared<StackElement<T>>();
        first = last = tmp;
        size = 0;
    }

    std::shared_ptr<T> pop(){
        if (!size){
            std::wcout << "stack is empty\n";
            return nullptr;
        }
        auto tmp = last->previous;
        if (size == 1){
            first = last;
            size = 0;
        } else {
            last->previous->previous->next = last;
            --size;
        }
        return tmp->value;
    }

    void push(T element) {
        auto tmp = std::make_shared<StackElement<T>>(element, last, last->previous);
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

    std::shared_ptr<T> top(){
        if (!size){
            std::wcout << "stack is empty\n";
            return nullptr;
        }
        return last->previous->value;
    }

    Iterator<T> begin() const{
        return Iterator<T>(self, first);
    }

    Iterator<T> end(){
        return Iterator<T>(self, last);
    }

    void insert(Iterator<T> it, T elem){
        auto tmp = std::make_shared<StackElement<T>>(elem, it.current->next, it.current);
        it.current->next->previous = tmp;
        it.current->next = tmp;
    }

    void erase(Iterator<T> it){
        it.current->previous->next = it.current->next;
        it.current->next->previous = it.current->previous;
    }

    void print(int i){
        auto it = this->begin();
        for (int k = 0; k < i; ++i){ ++it; }
        std::cout << *(it->current->value) << std::endl;
    }

    void print(){
        std::for_each(this->begin(), this->end(), [](int i){ print(i); });
    }

};

#endif //OOP_EXERCISE_05_STACK_H
