#pragma once

//clase que representa un nodo en una lista enlazada, con un elemento E y un puntero al siguiente nodo
//obtenido del trabajo en clase
//escrito por Julián Rodríguez

template <typename E>
class Node {
public:
	E element;
	Node<E>* next;

	Node(E element, Node<E>* next = nullptr) {
		this->element = element;
		this->next = next;
	}
	Node(Node<E>* next = nullptr) {
		this->next = next;
	}
};

