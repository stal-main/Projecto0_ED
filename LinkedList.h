#pragma once

#include <iostream>
#include <stdexcept>
#include "List.h"
#include "Node.h"

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>
class LinkedList : public List<E> {
private:
	Node<E>* head;
	Node<E>* current;
	Node<E>* tail;
	int size;

public:
	LinkedList() {
		head = current = tail = new Node<E>();
		size = 0;
	}
	~LinkedList() {
		clear();
		delete head;
	}
	void insert(E element) {
		current->next = new Node<E>(element, current->next);
		if (current == tail) {
			tail = tail->next;
		}
		size++;
	}
	void append(E element) {
		tail = tail->next = new Node<E>(element);
		size++;
	}
	E remove() {
		if (size == 0)
			throw runtime_error("List is empty");
		if (current == tail)
			throw runtime_error("No current element");
		E result = current->next->element;
		Node<E>* temp = current->next;
		current->next = temp->next;
		if (temp == tail)
			tail = current;
		delete temp;
		size--;
		return result;
	}
	void setElement(E element) {
		if (size == 0)
			throw runtime_error("List is empty");
		if (current == tail)
			throw runtime_error("No current element");
		current->next->element = element;
	}
	E getElement() {
		if (size == 0)
			throw runtime_error("List is empty");
		if (current == tail)
			throw runtime_error("No current element");
		return current->next->element;
	}
	void clear() {
		current = head->next;
		while (head->next != nullptr) {
			head->next = current->next;
			delete current;
			current = head->next;
		}
		size = 0;
		current = tail = head;
	}
	void goToStart() {
		current = head;
	}
	void goToEnd() {
		current = tail;
	}
	void goToPos(int pos) {
		if (pos > size || pos < 0)
			throw runtime_error("Invalid index.");
		current = head;
		for (int i = 0; i < pos; i++) {
			current = current->next;
		}
	}
	void next() {
		if (current != tail) {
			current = current->next;
		}
	}
	void previous() {
		if (current != head) {
			Node<E>* temp = head;
			while (temp->next != current)
				temp = temp->next;
			current = temp;
		}
	}
	bool atStart() {
		return current == head;
	}
	bool atEnd() {
		return current == tail;
	}
	int getPos() {
		Node<E>* temp = head;
		int pos = 0;
		while (temp != current) {
			temp = temp->next;
			pos++;
		}
		return pos;
	}
	int getSize() {
		return size;
	}
	void print() {
		cout << "[ ";
		Node<E>* temp = head->next;
		while (temp != nullptr) {
			cout << temp->element;
			if (temp->next != nullptr)
				cout << ", ";
			temp = temp->next;
		}
		cout << "]" << endl;
	}
};

