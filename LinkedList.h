#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#include <iostream>

template <class T>
class LinkedList
{
public:
    LinkedList()
    {
        this->head = nullptr;
        this->tail = nullptr;
        this->size = 0;
    }

    ~LinkedList()
    {
        Node *current = this->head;
        Node *next = nullptr;
        while (current != nullptr)
        {
            next = current->next;
            delete current;
            current = next;
        }
    }

    int getSize()
    {
        return this->size;
    }

    /**
     * Inserts a new node with the specified value at the beginning of the linked list.
     *
     * @param value The value to insert.
     */
    void push_front(T value)
    {
        Node *node = new Node(value, this->head);
        this->head = node;
        if (this->tail == nullptr)
        {
            this->tail = node;
        }
        this->size++;
    }

    T get(int index)
    {
        if (index < 0 || index >= this->size)
        {
            throw std::out_of_range("Index out of bounds");
        }

        Node *current = this->head;
        for (int i = 0; i < index; i++)
        {
            current = current->next;
        }
        return current->value;
    }

    T get_front()
    {
        if (this->head == nullptr)
        {
            throw std::out_of_range("Index out of bounds. List is empty.");
        }
        return this->head->value;
    }

    /**
     * Return index or -1 if not found
     *
     * @param value The value to be searched for.
     */
    int find(T value)
    {
        Node *current = this->head;
        int index = 0;
        while (current != nullptr)
        {
            if (current->value == value)
            {
                return index;
            }
            current = current->next;
            index++;
        }
        return -1;
    }

    void remove_front()
    {
        if (this->head == nullptr)
        {
            return;
        }

        Node *node = this->head;
        this->head = this->head->next;
        delete node;
        this->size--;
    }

    void remove(T value)
    {
        Node *current = this->head;
        Node *previous = nullptr;
        while (current != nullptr)
        {
            if (current->value == value)
            {
                if (previous == nullptr)
                {
                    this->head = current->next;
                }
                else
                {
                    previous->next = current->next;
                }
                delete current;
                this->size--;
                return;
            }
            previous = current;
            current = current->next;
        }
    }

    void removeAt(int index)
    {
        if (index < 0 || index >= this->size)
        {
            throw std::out_of_range("Index out of bounds");
        }

        Node *current = this->head;
        Node *previous = nullptr;
        for (int i = 0; i < index; i++)
        {
            previous = current;
            current = current->next;
        }

        if (previous == nullptr)
        {
            this->head = current->next;
        }
        else
        {
            previous->next = current->next;
        }
        delete current;
        this->size--;
    }

    void remove_duplicates()
    {
        Node *current = this->head;
        Node *previous = nullptr;
        while (current != nullptr)
        {
            previous = current;
            current = current->next;
            while (current != nullptr)
            {
                if (current->value == previous->value)
                {
                    previous->next = current->next;
                    delete current;
                    current = previous->next;
                    this->size--;
                }
                else
                {
                    previous = current;
                    current = current->next;
                }
            }
        }
    }

    void print()
    {
        Node *current = this->head;
        while (current != nullptr)
        {
            std::cout << current->value << ' ';
            current = current->next;
        }
        std::cout << std::endl;
    }

private:
    struct Node
    {
        Node(T value, Node *next = nullptr) : value(value), next(next) {}
        T value;
        Node *next;
    };
    Node *head;
    Node *tail;
    int size;
};

#endif
