#include <iostream>
#include <cassert>
#include "HashTable.h"

int main()
{
    // Create hash table
    HashTable<10> table;

    // Test adding elements to hash table
    table.add(42);
    table.add(13);
    table.add(7);
    table.add(19);
    table.add(23);

    // Test contains method
    assert(table.contains(42) == true);
    assert(table.contains(13) == true);
    assert(table.contains(7) == true);
    assert(table.contains(19) == true);
    assert(table.contains(23) == true);
    assert(table.contains(99) == false);

    // Test adding duplicate element
    table.add(42);
    assert(table.contains(42) == true);
    assert(table.contains(13) == true);
    assert(table.contains(7) == true);
    assert(table.contains(19) == true);
    assert(table.contains(23) == true);
    assert(table.contains(99) == false);

    // Test adding more elements to hash table
    table.add(11);
    table.add(29);
    table.add(17);
    std::cout << "Added 17\n";
    table.add(31);
    table.add(37);

    // Test contains method
    assert(table.contains(42) == true);
    assert(table.contains(13) == true);
    assert(table.contains(7) == true);
    assert(table.contains(19) == true);
    assert(table.contains(23) == true);
    assert(table.contains(99) == false);
    assert(table.contains(11) == true);
    assert(table.contains(29) == true);
    assert(table.contains(17) == true);
    assert(table.contains(31) == true);
    assert(table.contains(37) == true);

    // Test adding element to full hash table
    try
    {
        table.add(53);
        assert(false); // Should not reach this line
    }
    catch (const std::runtime_error &e)
    {
        assert(std::string(e.what()) == "HashTable is full and cannot insert new element.");
    }

    std::cout << "All tests passed!" << std::endl;

    return 0;
}