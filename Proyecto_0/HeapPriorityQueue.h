#pragma once

#include <stdexcept>
#include <iostream>
#include "PriorityQueue.h"
#include "Pair.h"
#include "MinHeap.h"

using std::runtime_error;

template <typename E>

class heapPriorityQueue : public priorityQueue<E> {

private:

	minHeap<Pair<int, E>>* pairs;

public:

	heapPriorityQueue(int max = DEFAULT_MAX) {

		pairs = new minHeap<Pair<int, E>>(max);
	}

	~heapPriorityQueue() {

		delete pairs;
	}

	void insert(E element, int priority) override {

		Pair<int, E> p(priority, element);

		pairs->insert(p);
	}

	E min() override {

		Pair<int, E> p = pairs->first();

		return p.value;
	}

	E removeMin() override {

		Pair<int, E> p = pairs->removeFirst();

		return p.value;

	}

	void clear() override {
		pairs->clear();
	}

	int getSize() const override {
		return pairs->getSize();
	}

	bool isEmpty() const override {
		return pairs->getSize() == 0;
	}

	void print() override {
		pairs->print();
	}
};

