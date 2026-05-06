#pragma once

//clase que representa un par de clave y valor, ademas de operadores de comparacion para comparar elementos a partir de sus llaves
//obtenido del trabajo en clase
//escrito por Kevin Solano

#include <ostream>

using std::ostream;

template <typename K, typename V>

class Pair {

public:

	K key;

	V value;

	Pair() {}

	Pair(K key) {

		this->key = key;
	}

	Pair(K key, V value) {

		this->key = key;
		this->value = value;

	}

	bool operator==(const Pair<K, V>& other) {
		return key == other.key;
	}

	bool operator!=(const Pair<K, V>& other) {
		return key != other.key;
	}

	bool operator<(const Pair<K, V>& other) {
		return key < other.key;
	}

	bool operator<=(const Pair<K, V>& other) {
		return key <= other.key;
	}

	bool operator>(const Pair<K, V>& other) {
		return key > other.key;
	}

	bool operator>=(const Pair<K, V>& other) {
		return key >= other.key;
	}

	friend ostream& operator<<(ostream& os, const Pair<K, V>& p) {
		os << "(" << p.key << ", " << p.value << ")";

		return os;
	}

};