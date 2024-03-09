#pragma once

template <typename T>
class PQNode {
private:
	T data;
	int priority;
	PQNode<T>* next_node;
public:
	PQNode<T>() : data(T()), next_node(nullptr) {}
	T getData() { return data; }
	PQNode<T>* getNext() { return next_node; }
	int getPriority() { return priority; }
	void setData(T data) { this->data = data; }
	void setNext(PQNode<T>* next_node) { this->next_node = next_node; }
	void setPriority(int p) { this->priority = p; }
};