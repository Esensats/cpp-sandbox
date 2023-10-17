#include <iostream>
#include <string>
#include <sstream>
#include <array>
#include <chrono>
#include <fstream>
#include <cmath>
#include <Windows.h>
#include <regex>

#pragma execution_character_set("utf-8")

struct FrequencyPair
{
    std::string word;
    int accumulator;
    bool exists;
};

template <size_t Size>
class Frequency
{
private:
    const char start_char;
    const char end_char;
    const int asciiSize;
    std::array<FrequencyPair, Size> table;
    size_t stored;
    size_t fail_count;
    size_t global_accumulator;
    size_t collision_count;
    size_t collision_iterations;

    /**
     * Example W = { A1, A2, A3, ... , AN }; N = |W|
     * Example A = {'h','e','l','l','o'}
     *
     * hash function f(x, y) for any possible letter x, y in word A should have these properties:
     * 1) f(x, y) != f(y, x) for x != y
     * 2) f(x, y) == f(y, x) for x == y
     * 3) f(x, y) >= 0 AND f(x, y) < |W|
     * 4) table[f(x, y)] exists -> f_probe(x, y, i) [should have all the above properties as well]
     *
     * probing cycle f_probe(x, y, i) for any 0 < i < |W| - 1 properties:
     * f_probe(x, y, i) should differ drastically from f_probe(x, y, i + 1)
     * f_probe(x, y, 0) = 0 [hypothetically]
     * f_probe(x, y, i) where i < |W| - 1
     * f_probe(x, y, |W| - 1) -> fail
     * f_probe(x, y, i) should be O(n) at worst
     * (|A_stored| < |W|) -> f_probe(x, y, i) should guarantee a spot
     * (|A_stored| = |W|) -> f_probe(x, y, i) should fail
     * (|A_stored| > |W|) shouldn't happen
     *
     * fail -> throw OR resize |W|
     *
     * Note: (x, y) is a simplification; the function should be able to take any set of letters, not just 2 letters.
     *
     * ---
     *
     * f(x, y) = x OR y
     * * f(1110, 0001) = 1111
     * * f(0001, 1110) = 1111 - not antisymmetrical
     * * f(1010, 0101) = 1111 - easy collision
     * * NO
     *
     * f(letters) = sum(u * (n - i)) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = x * 3 + y * 2 + z * 1
     * * f(4, 16, 10) = 12 + 32 + 10 = 54
     * * f(5, 16, 7) = 15 + 32 + 7 = 54 - NO (easy collision)
     *
     * f(letters) = sum(|Ascii| * (n - i) + u) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = (26 * 3 + x) + (26 * 2 + y) + (26 * 1 + z)
     * * f(4, 16, 10) = (26 * 3 + 4) + (26 * 2 + 16) + (26 * 1 + 10) = 82 + 68 + 36 = 186
     * * f(0, 25, 25) = (26 * 3 + 0) + (26 * 2 + 25) + (26 * 1 + 25) = 78 + 77 + 51 = 206
     * * f(25, 25, 25) = (26 * 3 + 25) + (26 * 2 + 25) + (26 * 1 + 25) = 103 + 77 + 51 = 231
     * * f(0, 0, 0) = (26 * 3 + 0) + (26 * 2 + 0) + (26 * 1 + 0) = 78 + 52 + 26 = 156
     * * 231 - 156 = 75
     * * f(25, 0, 25) = (26 * 3 + 25) + (26 * 2 + 0) + (26 * 1 + 25) = 103 + 52 + 51 = 206
     * * NO (easy collision (azz) == (zaz))
     *
     * f(letters) = sum(|Ascii| ^ (n - i) + u) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = (26^3 + x) + (26^2 + y) + (26^1 + z)
     * * f(4, 16, 10) = (26^3 + 4) + (26^2 + 16) + (26^1 + 10) = (17576 + 4) + (676 + 16) + (26 + 10)
     * * f(0, 25, 25) = (26^3 + 0) + (26^2 + 25) + (26^1 + 25) = (17576 + 0) + (676 + 25) + (26 + 25)
     * * NO (easy collision and too big numbers)
     *
     * f(letters) = mul((|Ascii| + u)) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = (26 + x) * (26 + y) * (26 + z)
     * * f(0, 25, 25) = 26 * 51 * 51 = 67626
     * * f(25, 0, 25) = 51 * 26 * 51 = 67626
     * * f(4, 16, 10) = (26 + 4) * (26 + 16) * (26 + 10) = 30 * 42 * 36 = 45360
     * * f(25, 25, 25) = 51 * 51 * 51 = 132651
     * * f(0, 0, 0) = 26 * 26 * 26 = 17576
     * * NO (easy collision (azz) == (zaz))
     *
     * f(letters) = sum((n - i) * (|Ascii| - u)) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = (3 * 26 - x) + (2 * 26 - y) + (1 * 26 - z)
     * * f(0, 25, 25) = (3 * 26 - 0) + (2 * 26 - 25) + (1 * 26 - 25) = 106
     * * f(25, 0, 25) = (3 * 26 - 25) + (2 * 26 - 0) + (1 * 26 - 25) = 106
     * * NO (easy collision (azz) == (zaz))
     *
     * f(letters) = sum((u + 1) ^ (n - i)) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = ((x+1) ^ 3) + ((y+1) ^ 2) + ((z+1) ^ 1)
     * * aaa - f(0, 0, 0) = 1^3 + 1^2 + 1^1 = 3
     * * aad - f(0, 0, 3) = 1^3 + 1^2 + 4^1 = 6
     * * aaz - f(0, 0, 25) = 1^3 + 1^2 + 26^1 = 28
     * * aba - f(0, 1, 0) = 1^3 + 2^2 + 1^1 = 6
     * * aza - f(0, 25, 0) = 1^3 + 26^2 + 1^1 = 678
     * * azz - f(0, 25, 25) = 1^3 + 26^2 + 26^1 = 703
     * * zaa - f(25, 0, 0) = 26^3 + 1^2 + 1^1 = 17578
     * * zaz - f(25, 0, 25) = 26^3 + 1^2 + 26^1 = 17603
     * * zza - f(25, 25, 0) = 26^3 + 26^2 + 1^1 = 18253
     * * zzz - f(25, 25, 25) = 26^3 + 26^2 + 26^1 = 18278
     * * azzz - f(0, 25, 25, 25) = 1^4 + 26^3 + 26^2 + 26^1 = 18279
     * * NO (collision like between "aad" and "aba")
     *
     * f(letters) = sum((u + [Ascii]) ^ (n - i)) where u is letter id, n is word length and i is letter position
     * f(x, y, z) = ((x+26) ^ 3) + ((y+26) ^ 2) + ((z+26) ^ 1)
     * * aaa - f(0, 0, 0) = 26^3 + 26^2 + 26^1 = 18278
     * * aad - f(0, 0, 3) = 26^3 + 26^2 + 29^1 = 18281
     * * aaz - f(0, 0, 25) = 26^3 + 26^2 + 51^1 = 18303
     * * aba - f(0, 1, 0) = 26^3 + 27^2 + 26^1 = 18331
     * * aza - f(0, 25, 0) = 1^3 + 26^2 + 1^1 = 678
     * * azz - f(0, 25, 25) = 1^3 + 26^2 + 26^1 = 703
     * * zaa - f(25, 0, 0) = 26^3 + 1^2 + 1^1 = 17578
     * * zaz - f(25, 0, 25) = 26^3 + 1^2 + 26^1 = 17603
     * * zza - f(25, 25, 0) = 26^3 + 26^2 + 1^1 = 18253
     * * zzz - f(25, 25, 25) = 51^3 + 51^2 + 51^1 = 135303
     * * azzz - f(0, 25, 25, 25) = 1^4 + 26^3 + 26^2 + 26^1 = 18279
     * * NO (collision like between "aad" and "aba")
     *
     * -----------THE BELOW WORKS!!!-----------
     *
     * 0 1 2 3 4 5 6 7 8 9      10 - 11 - 19 - 20 - 99 - 100 - 999 - 1000
     * # 0 1 2 3 . 6 7 8 9      0# - 00 - 09 - 1# - 99 - 0## - 999 - 0###
     * a b c . . . . x y z      ba - bb - bz - ca - zz - baa
     * # a b . . . . x y z      a# - aa - az - b# - zz - a## - zzz - a###
     *
     * a should be (0 + 1) - 1 = 0
     * z should be (25 + 1) - 1 = 25
     * aa should be 26^1 * (0 + 1) + (0 + 1) - 1 = 26
     * az should be 26^1 * (0 + 1) + (25 + 1) - 1 = 51
     * ba should be 26^1 * (1 + 1) + (0 + 1) - 1 = 52
     * bz should be 26^1 * (1 + 1) + (25 + 1) - 1 = 77
     * ca should be 26^1 * (2 + 1) + (0 + 1) - 1 = 78
     * ...
     * zz should be 26^1 * (25 + 1) + (25 + 1) - 1 = 701
     * aaa should be 26^2 * (0 + 1) + 26 * (0 + 1) + (0 + 1) - 1 = 702
     * zzz should be 26^2 * (25 + 1) + 26 * (25 + 1) + (25 + 1) - 1 = 18277
     * aaaa should be 26^3 * (0 + 1) + 26^2 * (0 + 1) + 26 * (0 + 1) + (0 + 1) - 1 = 18278
     *
     * f(word) = sum(ascii.size() ^ (word.length() - 1) * (word[i] + 1)) - 1
     *
     */

public:
    Frequency(const char &start_char = '!', const char &end_char = '~')
        : start_char(start_char), end_char(end_char),
          asciiSize(end_char - start_char + 1),
          stored(0), fail_count(0), global_accumulator(0),
          collision_count(0), collision_iterations(0)
    {
        FrequencyPair pair = {"", 0, false};
        table.fill(pair);
    }

    void reset()
    {
        stored = 0;
        global_accumulator = 0;
        fail_count = 0;
        collision_count = 0;
        collision_iterations = 0;
        FrequencyPair pair = {"", 0, false};
        table.fill(pair);
    }
    /* Frequency(const std::string &text)
    {
        FrequencyPair pair = {"", 0, false};
        table.fill(pair);
        countFrequency(text);
    } */
    size_t hash_base(const std::string &word) const
    {
        // f(word) = sum(ascii.size() ^ (word.length() - 1) * (word[i] + 1)) - 1
        // hash word
        size_t sum = 0;
        const unsigned int wordLength = word.length() - 1;
        const size_t power = std::pow(asciiSize, wordLength);
        for (unsigned short i = 0; i < wordLength; i++)
        {
            if (word[i] < start_char || word[i] > end_char)
                throw std::out_of_range("hash_base() is trying to check a character which is out of bounds");
            else
                sum += power * (word[i] - start_char);
        }
        sum--;
        return sum;
    }
    // FNV-1a hash function
    uint32_t fnv1a(const std::string &str, const uint32_t &seed = 0x811c9dc5) const
    {
        uint32_t hash = seed;
        for (const char c : str)
        {
            hash ^= static_cast<uint8_t>(c);
            hash *= 0x01000193;
        }
        return hash;
    }
    /**
     * Returns the spot if it's found, otherwise returns array size.
     */
    size_t hash(const std::string &word, bool &isNew, bool &isCollision, bool &isCollisionIteration) const
    {
        size_t h1 = fnv1a(word) % Size;
        isNew = false;
        isCollision = false;
        isCollisionIteration = false;
        if (!(table[h1].exists))
        {
            isNew = true;
            return h1;
        }
        if (table[h1].word == word)
        {
            return h1;
        }
        size_t h2 = (fnv1a(word, 0xdeadbeef) % (Size - 2)) + 1;
        size_t i = 0;
        while (i < Size - 1)
        {
            const size_t spot = (h1 + i * h2) % Size;
            if (!(table[spot].exists))
            {
                isNew = true;
                isCollision = true;
                isCollisionIteration = true;
                return spot;
            }
            if (table[spot].word == word)
            {
                isCollisionIteration = true;
                return spot;
            }
            i++;
        }
        return Size;
    }

    /**
     * If useRegex == true -> bool trim is ignored + very slow execution.
     * Else, skipNonWords skips "words" that start or end with non-alphabetic characters.
     */
    void countFrequencyFromText(const std::string &text, const bool &lowercase = false, const bool &skipNonWords = false, const bool &useRegex = false)
    {
        if (useRegex)
        {
            const std::regex word_regex("[a-zA-Z]+(-[a-zA-Z]+)*");
            auto words_begin = std::sregex_iterator(text.begin(), text.end(), word_regex);
            auto words_end = std::sregex_iterator();
            for (std::sregex_iterator i = words_begin; i != words_end; ++i)
            {
                std::string word = i->str();
                /* if (trim)
                {
                    word.erase(std::remove_if(word.begin(), word.end(), [](char c)
                                              { return !std::isalpha(c); }),
                               word.end());
                    if (word.empty())
                    {
                        continue;
                    }
                } */
                if (lowercase)
                    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                countFrequencyByWord(word);
            }
        }
        else
        {
            std::istringstream iss(text);
            std::string word;
            while (iss >> word)
            {
                if (skipNonWords && (!std::isalpha(word.front()) || !std::isalpha(word.back())))
                {
                    continue;
                }
                if (lowercase)
                    std::transform(word.begin(), word.end(), word.begin(), ::tolower);
                countFrequencyByWord(word);
            }
        }
    }
    void countFrequencyByWord(const std::string &word)
    {
        bool isNew = false;
        bool isCollision = false;
        bool isCollisionIteration = false;
        const size_t spot = hash(word, isNew, isCollision, isCollisionIteration);
        if (spot != Size)
        {
            table[spot].word = word;
            table[spot].accumulator++;
            table[spot].exists = true;
            global_accumulator++;
            if (isNew)
                stored++;
            if (isCollision)
                collision_count++;
            if (isCollisionIteration)
                collision_iterations++;
        }
        else
        {
            fail_count++;
        }
    }
    void print() const
    {
        for (size_t i = 0; i < Size; i++)
        {
            if (table[i].exists)
                std::cout << i << ' ' << table[i].word << ' ' << table[i].accumulator << '\n';
        }
    }
    size_t getGlobalAccumulator() const { return global_accumulator; }
    FrequencyPair getAt(const size_t &index) const
    {
        if (index < Size && index > 0)
            return table[index];
        else
            throw std::out_of_range("getAt() called with index out of bounds");
    }
    char getStartChar() const { return start_char; }
    char getEndChar() const { return end_char; }
    size_t getStoredSize() const { return stored; }
    size_t getFailedCount() const { return fail_count; }
    size_t getCollisions() const { return collision_count; }
    size_t getCollisionIterations() const { return collision_iterations; }
    size_t getSize() const { return Size; }
};

int main()
{
    SetConsoleOutputCP(65001);

    std::string line;

    std::ifstream textFile("article.txt");
    if (!textFile)
        return 1;

    Frequency freq = Frequency<800>();

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    while (std::getline(textFile, line))
    {
        freq.countFrequencyFromText(line, true, true);
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    freq.print();
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << " nanoseconds (1/1 000 000 000 of a second)" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " microseconds (1/1 000 000 of a second)" << std::endl;
    std::cout << "Time difference = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " milliseconds (1/1000 of a second)" << std::endl;

    std::cout << "Total words on input: " << freq.getGlobalAccumulator() << std::endl;
    std::cout << "Table size (total buckets): " << freq.getSize() << std::endl;
    std::cout << "Stored size (occupied buckets): " << freq.getStoredSize() << std::endl;
    std::cout << "Fails: " << freq.getFailedCount() << std::endl;
    std::cout << "Collisions (for unique words): " << freq.getCollisions() << std::endl;
    std::cout << "Collisions (iterations): " << freq.getCollisionIterations() << std::endl;

    textFile.close();

    return 0;
}
