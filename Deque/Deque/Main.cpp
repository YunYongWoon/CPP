#include<iostream>

template<typename T>
class Node {
private:
	T data;
	Node<T>* prev;
	Node<T>* next;
public:
	Node(T data = 0, Node<T>* prev = NULL, Node<T>* next = NULL);
	~Node();

	void setData(T _data);
	void setPrev(Node<T>* _prev);
	void setNext(Node<T>* _next);

	T getData();
	Node<T>* getPrev();
	Node<T>* getNext();
};

template<typename T>
class Deque {
	Node<T>* front;
	Node<T>* rear;
	int length = 0;
public:
	Deque<T>();
	~Deque<T>();

	void push_front(T data);
	void push_rear(T data);

	T pop_front();
	T pop_rear();

	bool is_empty();
	//void print_node();
};
// Node 클래스 구현

template<typename T>
Node<T>::Node(T _data, Node<T>* _prev, Node<T>* _next) : data(_data), prev(_prev), next(_next) {
}

template<typename T>
Node<T>::~Node() {
}

template<typename T>
void Node<T>::setData(T _data) {
	data = _data;
}

template<typename T>
void Node<T>::setPrev(Node<T>* _prev) {
	prev = _prev;
}

template<typename T>
void Node<T>::setNext(Node<T>* _next) {
	next = _next;
}

template<typename T>
T Node<T>::getData() {
	return data;
}

template<typename T>
Node<T>* Node<T>::getPrev() {
	return prev;
}

template<typename T>
Node<T>* Node<T>::getNext() {
	return next;
}
// Deque클래스 구현
template<typename T>
Deque<T>::Deque() {
	front = nullptr;
	rear = nullptr;
}

template<typename T>
Deque<T>::~Deque() {
	front = front->getNext();
	delete front->getPrev();
}

template<typename T>
void Deque<T>::push_front(T data) {
	Node<T>* newNode = new Node<T>(data, NULL, NULL);
	if (is_empty()) {
		front = rear = newNode;
		return;
	}

	front->setPrev(newNode);
	newNode->setNext(front);
	front = newNode;
}

template<typename T>
void Deque<T>::push_rear(T data) {
	Node<T>* newNode = new Node<T>(data, NULL, NULL);
	if (is_empty()) {
		front = rear = newNode;
		return;
	}

	rear->setNext(newNode);
	newNode->setPrev(rear);
	rear = newNode;
}

template<typename T>
T Deque<T>::pop_front() {
	T t = front->getData();
	front = front->getNext();
	delete front->getPrev();
	return t;
}

template<typename T>
T Deque<T>::pop_rear() {
	T t = rear->getData();
	rear = rear->getPrev();
	delete rear->getNext();
	return t;
}

template<typename T>
bool Deque<T>::is_empty() {
	if (front == NULL && rear == NULL)
		return true;
	else
		return false;
}

int main() {
	Deque<int> deq;

	deq.push_front(1);
	deq.push_rear(3); 
	deq.push_front(2);
	deq.push_rear(4);
}
