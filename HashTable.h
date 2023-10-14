#include <array>
#include <cstring>

template <std::size_t Size>
class HashTable
{
private:
    size_t stored;
    const size_t prime;
    std::array<int *, Size> array;
    bool debug;

    std::size_t hash1(const int &value)
    {
        return value % array.size();
    }
    std::size_t hash2(const int &value)
    {
        return prime - (value % prime);
    }
    /**
     * Returns the hash of value which corresponds to
     * a free index in the internal array.
     * If not found, returns HashTable Size.
     */
    std::size_t double_hash(const int &value, const std::string &method = "default")
    {
        if (debug)
            std::cout << method << " double_hash(" << value << ')';
        std::size_t result_index = hash1(value);
        // h1(k) == (empty spot OR the value spot) => return h1(k)
        if (array[result_index] == nullptr || *(array[result_index]) == value)
        {
            if (debug)
                std::cout << " hash1, index: " << result_index << '\n';
            return result_index;
        }
        // spot is occupied => start probing
        std::size_t i = 1;           // start from 1, because f(0) has to result in 0
        while (i < array.size() - 1) // i = size - 1 => probe has failed
        {
            /**
             * f(i) = i * h2(k)
             * Sequence is:
             * h(k) % size
             * h(k) + h2(k) % size
             * h(k) + 2(h2(k)) % size
             * h(k) + 3(h2(k)) % size
             * ...
             * h(k) + (size-2)(h2(k)) % size
             */
            // result is (h(k) + f(i)) % size
            result_index = (hash1(value) + i * hash2(value)) % array.size();
            // if probing results in an empty slot or finds the value, return the result
            if (array[result_index] == nullptr || *array[result_index] == value)
            {
                if (debug)
                    std::cout << " cycle, i: " << i << " index: " << result_index << " value: " << (array[result_index] != nullptr ? (*(array[result_index])) : -1) << std::endl;
                return result_index;
            }
            i++;
        }
        if (debug)
            std::cout << " Size, i: " << i << " index: " << result_index << '\n';
        return array.size(); // Value not found in hash table
    }

    /**
     * Uses Sieve of Eratosthenes algorithm to find a suitable
     * prime number during the construction of the hash table.
     */
    std::size_t findPrime() const
    {
        std::size_t prime = 5;
        std::size_t limit = array.size() - 1;
        if (limit <= 1)
        {
            prime = 2;
        }
        else if (limit == 2)
        {
            prime = 2;
        }
        else if (limit == 3)
        {
            prime = 3;
        }
        else
        {
            bool sieve[limit + 1];
            memset(sieve, true, sizeof(sieve));
            sieve[0] = false;
            sieve[1] = false;

            for (int p = 2; p * p <= limit; p++)
            {
                if (sieve[p] == true)
                {
                    for (int i = p * p; i <= limit; i += p)
                        sieve[i] = false;
                }
            }

            for (int p = limit; p >= 2; p--)
            {
                if (sieve[p])
                {
                    prime = p;
                    break;
                }
            }
        }
        return prime;
    }

    /**
     * Prints the entire internal array to the console.
     */
    void printArray() const
    {
        std::cout << "Internal array:\n";
        for (std::size_t i = 0; i < array.size(); i++)
        {
            if (array[i] != nullptr)
            {
                std::cout << "[" << i << "]: " << *(array[i]) << '\n';
            }
            else
            {
                std::cout << "[" << i << "]: null\n";
            }
        }
    }

public:
    HashTable() : stored(0), debug(true), array({}), prime(findPrime())
    {
        array.fill(nullptr);
    }
    ~HashTable()
    {
        // Free memory allocated for stored elements
        for (std::size_t i = 0; i < array.size(); i++)
        {
            if (array[i] != nullptr)
            {
                delete array[i];
                array[i] = nullptr;
            }
        }
    }

    /* std::size_t find(const int &value) {
        const std::size_t index1 = hash1(value);
        if(index1) {

        }
    } */

    bool contains(const int &value)
    {
        // Find index of value in hash table
        const std::size_t index = double_hash(value, "contains");

        // Check if value is in hash table
        if (index < array.size() && array[index] != nullptr && *(array[index]) == value)
        {
            return true;
        }

        // Value not found in hash table
        return false;
    }

    void add(const int &value)
    {
        // Check if value already exists in hash table
        if (contains(value))
        {
            return;
        }

        // Find empty slot in hash table
        const std::size_t index = double_hash(value, "add");

        // Check if hash table is full
        if (index == array.size())
        {
            if (debug)
                printArray();
            throw std::runtime_error("HashTable is full and cannot insert new element.");
        }

        // Store value in hash table
        array[index] = new int(value);

        // Increment stored count
        stored++;
    }
};