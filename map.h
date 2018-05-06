#ifndef __MY_MAP__
#define __MY_MAP__

#include <vector>
#include <iostream>
#include <memory>
#include <array>
#include <stdexcept>

#define NUM_BINS 2 // number of duplicates allowed for a single index
#define STARTING_DIVISOR 2

template<typename Key, typename T>
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
        int index = converter(key);
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

    T& get(const Key& key)
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
        std::cout << "m_divisor is now " << m_divisor << "\n";
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
    int find(const Key& key, int index)
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

    bool containsValue(const std::shared_ptr<KVStruct>& ptr)
    {
        if (static_cast<bool>(ptr))
        {
            return true;
        }
        return false;
    }

    int converter(const Key& key)
    {
        if (key < 0)
        {
            return (key * -1) % m_divisor;
        }
        return key % m_divisor;
    }
};

#endif