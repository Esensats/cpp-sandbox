#include <iostream>
#include "LinkedList.h"

void test_push_front()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    std::cout << "Expected output: 3 2 1\n";
    list.print();
}

void test_get()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int value = list.get(1);

    std::cout << "Expected output: 2\n";
    std::cout << "Actual output: " << value << '\n';
}

void test_get_front()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int value = list.get_front();

    std::cout << "Expected output: 3\n";
    std::cout << "Actual output: " << value << '\n';
}

void test_find()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    int index = list.find(2);

    std::cout << "Expected output: 1\n";
    std::cout << "Actual output: " << index << '\n';
}

void test_remove_front()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.remove_front();

    std::cout << "Expected output: 2 1\n";
    list.print();
}

void test_remove()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.remove(2);

    std::cout << "Expected output: 3 1\n";
    list.print();
}

void test_removeAt()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(3);

    list.removeAt(1);

    std::cout << "Expected output: 3 1\n";
    list.print();
}

void test_remove_duplicates()
{
    LinkedList<int> list;

    list.push_front(1);
    list.push_front(2);
    list.push_front(2);
    list.push_front(3);
    list.push_front(3);
    list.push_front(3);

    list.remove_duplicates();

    std::cout << "Expected output: 3 2 1\n";
    list.print();
}

int main()
{
    test_push_front();
    test_get();
    test_get_front();
    test_find();
    test_remove_front();
    test_remove();
    test_removeAt();
    test_remove_duplicates();

    return 0;
}