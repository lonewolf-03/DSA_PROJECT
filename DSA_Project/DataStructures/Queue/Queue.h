#pragma once
#define max_size 1000

#include "../Node.h"
#include <iostream>

using namespace std;


template <typename T>
class LinkedListQueue {
private:
	int count;
	Node<T>* front;
	Node<T>* rear;
public:
	LinkedListQueue() : count(0), front(nullptr), rear(nullptr) {}

	// copy constructor
	LinkedListQueue(const LinkedListQueue& Q) : count(0), front(nullptr), rear(nullptr) {
		Node<T>* node = Q.front;
		while (node) {
			this->enqueue(node->getData());
			node = node->getNext();
		}
	}

	// overloading assignement operator
	void operator =(const LinkedListQueue& Q) {
		this->clear();
		count = 0;
		front = nullptr;
		rear = nullptr;
		Node<T>* node = Q.front;
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

	bool enqueue(T item) {
		if (isFull()) {
			return false;
		}

		Node<T>* new_node = new Node<T>(item,nullptr);
		new_node->setData(item);
		new_node->setNext(nullptr);

		if (isEmpty()) {
			rear = new_node;
			front = rear;
		}
		else {
			rear->setNext(new_node);
			rear = new_node;
		}

		count++;

		return true;
	}

	bool dequeue(T& item) {
		if (isEmpty()) {
			return false;
		}

		item = front->getData();
		Node<T>* temp = front;
		front = front->getNext();
		delete temp;

		count--;
		return true;
	}

	T getElementAt(int j) {
		T element = nullptr; // NOTE: this works only when T is an integral type or any pointer type
		if ((j >= 0) && (j < count)) {
			Node<T>* node = rear;
			int c = 0;
			for (int i = 0; i < j; i++) {
				node = node->getNext();
			}

			element = node->getData();
		}

		return element;
	}

	void clear() {
		Node<T>* node = front;
		Node<T>* temp;
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
		Node<T>* node = front;
		if (node) {
			while (node && node->getNext()) {
				std::cout << "[ " << node->getData() << "]";
				node = node->getNext();
			}

			std::cout << "[ " << node->getData() << "]\n";
		}
	}

	~LinkedListQueue() {
		this->clear();
	}

};