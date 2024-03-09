#pragma once
#define max_size 1000

#include "PQNode.h"
#include <iostream>

template <typename T>
class PriorityQueue {
private:
	int count;
	PQNode<T>* front;
	PQNode<T>* rear;
public:
	PriorityQueue() : count(0), front(nullptr), rear(nullptr) {}

	// copy constructor
	PriorityQueue(const PriorityQueue& Q) : count(0), front(nullptr), rear(nullptr) {
		PQNode<T>* node = Q.front;
		while (node) {
			this->enqueue(node->getData());
			node = node->getNext();
		}
	}

	// overloading assignement operator
	void operator =(const PriorityQueue& Q) {
		this->clear();
		count = 0;
		front = nullptr;
		rear = nullptr;
		PQNode<T>* node = Q.front;
		while (node) {
			this->enqueue(node->getData());
			node = node->getNext();
		}
	}

	bool isEmpty() const { return (count == 0); }

	bool isFull() const { return (count == max_size); }


	bool peakFront(T& item) const {
		if (!isEmpty()) {
			item = front->getData();
			return true;
		}

		return false;
	}

	bool peakRear(T& item) const {
		if (!isEmpty()) {
			item = rear->getData();
			return true;
		}

		return false;
	}

	int getCount() const { return count; }

	bool enqueue(T item, int priority) {
		if (isFull()) {
			return false;
		}

		PQNode<T>* new_node = new PQNode<T>;
		new_node->setData(item);
		new_node->setNext(nullptr);
		new_node->setPriority(priority);

		if (isEmpty()) {
			rear = new_node;
			front = rear;
		}
		else {
			PQNode<T>* node = rear;
			while (node->getNext() && node->getNext()->getPriority() >= priority) {
				node = node->getNext();
			}

			new_node->setNext(node->getNext());
			node->setNext(new_node);

			if (!node->getNext()) {
				rear = new_node;
			}
		}

		count++;

		return true;
	}

	bool dequeue(T& item) {
		if (isEmpty()) {
			return false;
		}

		item = front->getData();
		PQNode<T>* temp = front;
		front = front->getNext();
		delete temp;

		count--;
		return true;
	}

	void clear() {
		PQNode<T>* node = front;
		PQNode<T>* temp;
		if (node) {
			while (node && node->getNext()) {
				temp = node;
				node = node->getNext();
				delete temp;
			}

			delete node;
		}

		front = nullptr;
		rear = nullptr;
	}

	void Print() const {
		PQNode<T>* node = front;
		if (node) {
			while (node && node->getNext()) {
				std::cout << "[ " << node->getData() << ", " << node->getPriority() << " ]";
				node = node->getNext();
			}

			std::cout << "[ " << node->getData() << ", " << node->getPriority() << " ]";
		}
	}

	T getElementAt(int j) {
		T element = nullptr; // NOTE: this works only when T is an integral type or any pointer type
		if ((j >= 0) && (j < count)) {
			PQNode<T>* node = rear;
			int c = 0;
			for (int i = 0; i < j; i++) {
				node = node->getNext();
			}

			element = node->getData();
		}

		return element;
	}

	~PriorityQueue() {
		this->clear();
	}

};