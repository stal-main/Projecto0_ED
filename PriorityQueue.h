#pragma once

//metodos de la clase abstracta priorityQueue
//obtenido del trabajo en clase
//escrito por Kevin Solano

template <typename E>

class priorityQueue {

public:

    priorityQueue() {}

    priorityQueue(const priorityQueue<E>&) = delete;

    void operator=(const priorityQueue<E>&) = delete;

    virtual ~priorityQueue() {}

    virtual void insert(E element, int priority) = 0;

    virtual E min() = 0;

    virtual E removeMin() = 0;

    virtual void clear() = 0;

    virtual int getSize() const = 0;

    virtual bool isEmpty() const = 0;

    virtual void print() = 0;
};