#ifndef LINKED_QUEUE_
#define LINKED_QUEUE_
#include "Node.h"
#include <iostream>
using namespace std;

template <typename T>
class LinkedQueue
{
private:
	Node<T>* backPtr;
	Node<T>* frontPtr;
public:
	LinkedQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peek(T& frntEntry)  const;
	void print() const;
	int getCount() const;
	~LinkedQueue();
};

template <typename T>
LinkedQueue<T>::LinkedQueue()
{
	backPtr = nullptr;
	frontPtr = nullptr;

}

template <typename T>
bool LinkedQueue<T>::isEmpty() const
{
	return (frontPtr == nullptr);
}

template <typename T>
bool LinkedQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);
	// Insert the new node
	if (isEmpty())	//special case if this is the first node to insert
		frontPtr = newNodePtr; // The queue is empty
	else
		backPtr->setNext(newNodePtr); // The queue was not empty

	backPtr = newNodePtr; // New node is the last node now
	return true;
} // end enqueue

template <typename T>
bool LinkedQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = frontPtr;
	frntEntry = frontPtr->getItem();
	frontPtr = frontPtr->getNext();
	// Queue is not empty; remove front
	if (nodeToDeletePtr == backPtr)	 // Special case: last node in the queue
		backPtr = nullptr;

	// Free memory reserved for the dequeued node
	delete nodeToDeletePtr;

	return true;
}

template <typename T>
bool LinkedQueue<T>::peek(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = frontPtr->getItem();
	return true;

}

template <typename T>
void LinkedQueue<T>::print() const
{
	Node<T>* curr = frontPtr;
	while (curr != nullptr)
	{
		cout << curr->getItem() << endl;
		curr = curr->getNext();
	}
}

template <typename T>
int LinkedQueue<T>::getCount() const
{
	int count = 0;
	Node<T>* curr = frontPtr;
	while (curr != nullptr)
	{
		count++;
		curr = curr->getNext();
	}
	return count;
}

template <typename T>
LinkedQueue<T>::~LinkedQueue()
{
	//Free all nodes in the queue
	T temp;
	while (dequeue(temp));
}

#endif