#include <algorithm>
#include <memory>
#include <iostream>
#include "stack.h"

template<class T>
Triangle<T>::Triangle(vertex_t<T> v1, vertex_t<T> v2, vertex_t<T> v3) {
    vertex[0] = v1;
    vertex[1] = v2;
    vertex[2] = v3;
}

template<class T>
Triangle<T>::Triangle(){
    vertex[0] = {0, 0};
    vertex[1] = {0, 0};
    vertex[2] = {0, 0};
}

template<class T>
double Triangle<T>::area() const{
    return 0.5 * std::abs((vertex[1].first - vertex[0].first) * (vertex[2].second - vertex[0].second) -
                          (vertex[2].first - vertex[0].first) * (vertex[1].second - vertex[0].second));
}

template<class U>
std::ostream& operator<< (std::ostream& out, const Triangle<U>& e){
    std::cout << "triangle (" << e.vertex[0].first << ", " << e.vertex[0].second << "), " <<
              "(" << e.vertex[1].first << ", " << e.vertex[1].second << "), " <<
              "(" << e.vertex[2].first << ", " << e.vertex[2].second << ")";
    return out;
}

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

template<class T>
void Stack<T>::push(T element) {
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

template<class T>
std::shared_ptr<T> Stack<T>::top() const{
    if (!size){
        std::wcout << "stack is empty\n";
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
void Stack<T>::insert(Iterator<T> it, T elem){
    auto tmp = std::make_shared<StackElement<T>>(elem, it.current->next, it.current);
    it.current->next->previous = tmp;
    it.current->next = tmp;
}

template<class T>
void Stack<T>::erase(Iterator<T> it){
    it.current->previous->next = it.current->next;
    it.current->next->previous = it.current->previous;
}

template<class T>
void Stack<T>::print(int i) const{
    auto it = this->begin();
    for (int k = 0; k < i; ++i){ ++it; }
    std::cout << *(it.current->value) << std::endl;
}

template<class T>
void Stack<T>::print() const{
    for (Iterator<T> it = this->begin(); it != this->end(); ++it){
        std::cout << *it << std::endl;
    }
}

template struct Stack<Triangle<int>>;
template struct Triangle<int>;


