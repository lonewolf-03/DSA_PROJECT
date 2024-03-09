#pragma once


template <class type>
class Node {
private:
	
public:
	int weight;
	type data;
	Node<type>* next;
	Node(type v, Node<type>* n, int w = 0) : data(type()), next(nullptr) {
		data = v;
		next = n;
		weight = w;
	}

	type getData() { return data; }
	Node<type>* getNext() { return next; }
	void setData(type data) { this->data = data; }
	void setNext(Node<type>* next_node) { this->next = next_node; }


};