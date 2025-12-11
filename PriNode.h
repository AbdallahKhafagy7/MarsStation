#pragma once
template <class T>
class PriNode {
    T item;
    int priority;
    PriNode<T>* next;
public:
    PriNode(const T& i, int p) : item(i), priority(p), next(nullptr) {}
    T getItem() const { return item; }
    int getPri() const { return priority; }
    PriNode<T>* getNext() const { return next; }
    void setNext(PriNode<T>* n) { next = n; }
};