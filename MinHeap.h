#pragma once

//clase que implementa un minHeap con los metodos de insert, first, removeFirst, remove, clear, getSize y print
//obtenido del trabajo en clase
//escrito por Kevin Solano

#define DEFAULT_MAX 1024

#include <iostream>
#include <stdexcept>

using std::runtime_error;
using std::cout;
using std::endl;

template <typename E>


class minHeap {

private:

	E* elements;

	int max;

	int size;

	int leftChild(int pos) {
		return 2 * pos + 1;

	}

	int rightChild(int pos) {
		return 2 * pos + 2;
	}

	int parent(int pos) {
		return (pos - 1) / 2;
	}

	void swap(int pos1, int pos2) {

		E temp = elements[pos1];

		elements[pos1] = elements[pos2];
		elements[pos2] = temp;
	}

	void siftUp(int pos) {

		while (pos > 0 and elements[pos] < elements[parent(pos)]) {

			swap(pos, parent(pos));

			pos = parent(pos);
		}
	}

	bool isLeaf(int pos) {

		return leftChild(pos) >= size;
	}

	int minChild(int pos) {

		if (rightChild(pos) < size and elements[rightChild(pos)] < elements[leftChild(pos)]) {
			return rightChild(pos);
		}

		return leftChild(pos);
	}

	void siftDown(int pos) {

		while (!isLeaf(pos) and elements[minChild(pos)] < elements[pos]) {

			int minPos = minChild(pos);

			swap(pos, minPos);

			pos = minPos;

		}
	}

public:

	minHeap(int max = DEFAULT_MAX) {

		elements = new E[max];

		this->max = max;

		size = 0;
	}

	~minHeap() {

		delete[] elements;
	}

	void insert(E element) {

		if (size == max) {
			throw runtime_error("Heap is full");
		}

		elements[size] = element;

		siftUp(size);

		size++;
	}

	E first() {
		if (size == 0) {
			throw runtime_error("Heap is empty");
		}
		return elements[0];

	}

	E removeFirst() {
		return remove(0);
	}

	E remove(int pos) {
		if (size == 0) {
			throw runtime_error("Heap is empty");
		}
		if (pos < 0 || pos >= size) {
			throw runtime_error("Invalid index");
		}

		swap(pos, size - 1);

		size--;

		siftDown(pos);

		siftUp(pos);

		return elements[size];

	}

	//retorna el elemento en la posicion dada sin eliminarlo del heap
	E getAt(int pos) const {

		if (pos < 0 || pos >= size)
			throw runtime_error("Invalid index");

		return elements[pos];
	}

	void clear() {

		size = 0;
	}

	int getSize() const {
		return size;
	}

	void print() {

		cout << "[ ";

		for (int i = 0; i < size; i++) {

			cout << elements[i];

			if (i != size - 1) {
				cout << ", ";
			}
		}

		cout << " ]" << endl;
	}



};

