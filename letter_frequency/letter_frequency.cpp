#include <iostream>
#include <string>
#include <array>
#include <chrono>
#include <fstream>

template <size_t Size>
class Frequency
{
private:
    std::array<int, Size> table;
    size_t offset;

    /**
     * Takes symbol.
     * Returns its spot in the array if it fits, otherwise returns the array's size
     */
    size_t hash(const char &c)
    {
        const size_t slot = (size_t)(c - ' ' + offset);
        if (slot < 0 || slot >= Size)
            return Size;
        return slot;
        // c = 01101111 -> 111
        // a = 01101101 -> 109
    }
    /**
     * alphabet[0] = 1 // a
     * alphabet[1] = 15 // b
     * ...
     * alphabet[25] = 0 // z
     *
     * ---
     *
     * alphabet[0] = alphabet[0] + 1
     */

public:
    Frequency(const size_t &offset = 0)
    {
        table.fill(0);
        this->offset = offset;
    }
    Frequency(const std::string &text, const size_t &offset = 0)
    {
        table.fill(0);
        this->offset = offset;
        countFrequency(text);
    }
    void countFrequency(const std::string &text)
    {
        for (int i = 0; i < text.length(); i++)
        {
            table[hash(text[i])]++;
        }
    }
    void reset()
    {
        table.fill(0);
    }
    void printFrequencySlow(const std::string &text)
    {
        // std::cout << "printFrequencySlow() called, offset = " << offset << ", size = " << Size << '\n';
        for (char c = ' ' + offset; c < Size + ' '; ++c)
        {
            int frequency = 0;
            for (unsigned long long i = 0; i < text.length(); ++i)
            {
                if (text[i] == c)
                {
                    frequency++;
                }
            }
            // std::cout << c << ' ' << frequency << '\n';
        }
        // std::cout << "printFrequencySlow() ended\n";
    }
    void print() const
    {
        for (size_t i = 0; i < Size; i++)
        {
            std::cout << (char)(i + ' ') << ' ' << table[i] << '\n';
        }
    }
};

int main()
{
    std::string text;

    std::ifstream textFile("text.txt");
    if (!textFile)
        return 1;

    Frequency freq = Frequency<'~' - ' ' + 1>();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (std::getline(textFile, text))
    {
        freq.countFrequency(text);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference fast = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl;
    freq.print();

    // std::cout << " ------ \n";

    // freq.reset();

    /* std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (std::getline(textFile, text))
    {
        freq.printFrequencySlow(text);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    std::cout << "Time difference slow = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds" << std::endl; */

    // std::cout << "1st char: " << (char)(0) << '\n';

    textFile.close();
    return 0;
}
