#pragma once

#include "../../DataStructures/Node.h"
#include "ListADT.h"
#include <iostream>

using namespace std;
#define max_size 1000
#define null NULL 
template <typename T>
//class List {
//	vector<T> v;
//	Node<T>* front = null;
//	Node<T>* tail = null;
//public:
//	int count = 0;
//
//	void insert_beginning(T val) {
//		Node<T>* n = new Node<T>(val, null);
//		n->setNext(front);
//		front = n;
//	}
//	T elementAt(int index) {
//		return v[index];
//		/*Node<T>* n = front;
//		while (index--) {
//			n = n->next;
//		}
//		return n->data;*/
//	}
//	void clear() {
//		/*while (!isEmpty()) {
//			deQueue();
//		}
//		count = 0;*/
//		v.clear();
//	}
//	void insert(T val) {
//		v.push_back(val);
//		/*if (front == null) {
//			front = tail = new Node<T>(val, null);
//		}
//		else {
//			tail->next = new Node<T>(val, null);
//			tail = tail->next;
//		}
//		count++;*/
//	}
//	void insert(T val, int weight)
//	{
//		Node<T>* temp, * q;
//		temp = new Node<T>(val, null, weight);
//		if (front == NULL || weight > front->weight)
//		{
//			temp->next = front;
//			front = temp;
//		}
//		else
//		{
//			q = front;
//			while (q->next != NULL && q->next->weight >= weight)
//				q = q->next;
//			temp->next = q->next;
//			q->next = temp;
//		}
//		count++;
//	}
//	T deQueue()
//	{
//		Node<T>* temp;
//		if (front != NULL)
//		{
//			temp = front;
//			T data = temp->data;
//			front = front->next;
//			//free(temp);
//			delete temp;
//			count--;
//			return data;
//		}
//	}
//	bool isEmpty() {
//		return (count == 0);
//	}
//	T top() {
//		return front->data;
//	}
//	void Delete(T& val) {
//		if (front != null && front->data == val) {
//			Node<T>* a = front->next;
//			delete front;
//			front = a;
//			count--;
//			return;
//		}
//		Node<T>* prev = null;
//		Node<T>* t = front;
//		while (t != null) {
//			prev = t;
//			t = t->next;
//			if (t != null && t->data == val) {
//				prev->next = t->next;
//				delete t;
//				count--;
//				break;
//			}
//		}
//	}
//};

class List {
	//vector<T> v;
	Node<T>* front = null;
	Node<T>* tail = null;
public:
	int count = 0;
	bool isEmpty() {
		return (count == 0 && front == null);
	}

	void insert_beginning(T val) {
		Node<T>* n = new Node<T>(val, null);
		n->setNext(front);
		front = n;
	}
	T elementAt(int index) {
		//return v[index];
		Node<T>* n = front;
		while (index--) {
			n = n->next;
		}
		return n->data;
	}
	void clear() {
		while (!isEmpty()) {
			deQueue();
		}
		count = 0;
	}
	void insert(T val) {
		if (front == null) {
			front = tail = new Node<T>(val, null);
		}
		else {
			tail->next = new Node<T>(val, null);
			tail = tail->next;
		}
		count++;
	}

	void insert(T val, int weight)
	{
		Node<T>* temp, * q;
		temp = new Node<T>(val, null, weight);
		if (front == NULL) {
			temp->next = front;
			front = tail = temp;
		}
		else if (weight > front->weight) {
			temp->next = front;
			front = temp;
		}
		else {
			q = front;
			while (q->next != NULL && q->next->weight >= weight)
				q = q->next;
			temp->next = q->next;

			q->next = temp;
			if (q == tail) {
				tail = q->next;
			}
		}
		/*if (front == NULL || weight > front->weight)
		{
			temp->next = front;
			front = temp;
		}
		else
		{
			q = front;
			while (q->next != NULL && q->next->weight >= weight)
				q = q->next;
			temp->next = q->next;
			q->next = temp;
		}*/

		count++;
	}

	T deQueue()
	{
		Node<T>* temp;
		if (front != NULL)
		{
			temp = front;
			T data = temp->data;
			if (front->next == NULL) {
				tail = front->next;
			}
			front = front->next;

			//free(temp);
			delete temp;
			count--;
			return data;
		}

	}

	T top() {
		return front->data;
	}
	void Delete(T& val) {
		if (front != null && front->data == val) {
			Node<T>* a = front->next;
			if (front == tail) {
				tail = front->next;
			}
			delete front;
			front = a;
			count--;
			return;
		}
		Node<T>* prev = null;
		Node<T>* t = front;
		while (t != null) {
			prev = t;
			t = t->next;
			if (t != null && t->data == val) {
				prev->next = t->next;
				if (t == tail) {
					tail = prev;
				}
				delete t;
				count--;
				break;
			}
		}
	}
};


