// MAP
// This program uses a simple function to convert a key of any primitive type into a semi-unique
// integer and promises O(1) access time to data given its key. The data is structured as a vector
// of arrays. The size of the arrays in the vector are referred to as "bins." The number of bins is
// the number of duplicates expected to be produced from the conversion function. Since integers
// and characters are signed, this number is 2. The number of bins for floats is much higher because
// the range of values a float can take include many orders of magnitude.
// 
// NEW PATTERNS IMPLEMENTED
// 1) Reduced number of magic numbers by using #define preprocessor macros
// 2) Gave #ifndef macros more unique names to reduce probability of naming collisions
// 3) Made functions const where possible
// 
// Author: Chris Lee
// Date: May 6th, 2018
// Last edited: May 7th, 2018

#ifndef __MY_MAP__
#define __MY_MAP__

#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <stdexcept>

#define NUM_BINS 46 // number of duplicates allowed for a single index
#define STARTING_DIVISOR 2

template<typename Key, typename T, class Hash>
class MyMap
{
private:
    struct KVStruct
    {
        KVStruct(const Key& key, const T& value): m_key(key), m_value(value) {}
        Key m_key;
        T m_value; 
    };
public: 
    MyMap(): m_divisor(STARTING_DIVISOR) 
    {
        m_data.resize(STARTING_DIVISOR);
        for (int index = 0; index < m_divisor; ++index)
        {
            for (int bin = 0; bin < NUM_BINS; ++bin)
            {
                m_data[index][bin] = std::shared_ptr<KVStruct>();
            }
        }
    }
    bool set(const Key& key, const T& value)
    {
        uint64_t hashnumber = Hash(key);
        int index = converter(hashnumber);
        int bin = find(key, index);
        if (bin >= 0)
        {
            m_data[index][bin] = std::make_shared<KVStruct>(key, value);
            return true;
        }
        else
        {
            while(true)
            {
                remap();
                index = converter(key);
                bin = find(key, index);
                if (bin >= 0)
                {
                    m_data[index][bin] = std::make_shared<KVStruct>(key, value);
                    return true;
                }
                else
                {
                    continue; // remap once again
                }
            }
        }
        return false;
    }

    T& get(const Key& key) const
    {
        int index = converter(key);
        int bin = find(key, index);

        if (bin >= 0)
        {
            return m_data[index][bin]->m_value;
        }
        else
        {
            throw std::invalid_argument( "Key not found" );
        }
    }

private:
    std::vector<std::array<std::shared_ptr<KVStruct>, NUM_BINS>> m_data;
    int m_divisor;

    bool remap()
    {
        m_divisor = m_divisor * 2; 
        auto oldVec = m_data;
        m_data.clear();

        // set vals to null 
        m_data.resize(m_divisor);
        for (int index = 0; index < m_divisor; ++index)
        {
            for (int bin = 0; bin < NUM_BINS; ++bin)
            {
                m_data[index][bin] = std::shared_ptr<KVStruct>();
            }
        }

        // copy vals using new divisor
        for (int index = 0; index < m_divisor / 2; ++index)
        {
            for (int bin = 0; bin < NUM_BINS; ++bin)
            {
                if (containsValue(oldVec[index][bin]))
                {
                    auto currentPtr = oldVec[index][bin];
                    int newIndex = converter(currentPtr->m_key);
                    int newBin = find(currentPtr->m_key, newIndex);
                    if (newBin >= 0)
                    {
                        m_data[newIndex][newBin] = oldVec[index][bin];
                    }
                    else
                    {
                        // If we still can't fit all of our values in, this current divisor has failed
                        // and we need to change it again, so we'll reset it
                        m_data = oldVec; 
                        return false; 
                    }
                }
            }            
        }
        return true;
    }

    // -1 signifies no matching keys and no empty spots to place a new one
    int find(const Key& key, int index) const
    {
        for (int bin = 0; bin < NUM_BINS; ++bin)
        {
            if (!containsValue(m_data[index][bin]))
            {
                return bin;
            }
            if (m_data[index][bin]->m_key == key)
            {
                return bin;
            }
        }
        return -1;
    }

    bool containsValue(const std::shared_ptr<KVStruct>& ptr) const
    {
        if (static_cast<bool>(ptr))
        {
            return true;
        }
        return false;
    }

    int converter(const Key& key) const
    {
        Key temp = key;
        if (temp < 0)
        {
            temp = temp * -1;
        }

        int count = 0; // since chars can't go above 255 we need to make a cutoff
        int limit = 45;
        while ((temp < 1000000) && (count < limit))
        {
            temp = temp * 10;
            ++count;
        }

        return static_cast<int>(temp) % m_divisor;
    }
};

#endif