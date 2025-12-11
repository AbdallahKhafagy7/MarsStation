#pragma once
#include "./PriNode.h"
#include <iostream>
using namespace std;

template <class T>
class PriQueue
{
    PriNode<T>* head;

public:
    PriQueue() : head(nullptr) {}

    ~PriQueue() {
        T dummy;
        int p;
        while (dequeue(dummy, p)); // safely delete all nodes
    }

    void enqueue(const T& data, int priority) {
        PriNode<T>* newNode = new PriNode<T>(data, priority);

        // Insert at head if list is empty or priority is highest
        if (!head || priority > head->getPri()) {
            newNode->setNext(head);
            head = newNode;
            return;
        }

        // Find correct position
        PriNode<T>* current = head;
        while (current->getNext() && priority <= current->getNext()->getPri()) {
            current = current->getNext();
        }

        newNode->setNext(current->getNext());
        current->setNext(newNode);
    }

    bool dequeue(T& topEntry, int& pri) {
        if (isEmpty())
            return false;

        topEntry = head->getItem();
        pri = head->getPri();

        PriNode<T>* temp = head;
        head = head->getNext();
        delete temp;
        return true;
    }

    bool peek(T& topEntry, int& pri) const {
        if (isEmpty())
            return false;

        topEntry = head->getItem();
        pri = head->getPri();
        return true;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void print() const {
        PriNode<T>* current = head;
        while (current) {
            cout << current->getItem() << " (priority: " << current->getPri() << ") -> ";
            current = current->getNext();
        }
        cout << "NULL\n";
    }

    int getCount() const {
        int count = 0;
        PriNode<T>* current = head;
        while (current) {
            count++;
            current = current->getNext();
        }
        return count;
    }
};