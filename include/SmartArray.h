#pragma once
#include <stdexcept>
#include <ostream>
#include <cstddef>
#include <fstream>
#include <string>

template<typename T = int>
class SmartArray {
public:
    // konstrukcja
    SmartArray() { ReAlloc(2); }
    explicit SmartArray(size_t size) {
        ReAlloc(size);
        m_Size = size;
    }
    SmartArray(size_t size, const T& defaultValue) {
        ReAlloc(size);
        for (size_t i = 0; i < size; ++i)
            m_Data[i] = defaultValue;
        m_Size = size;
    }

    // destruktor
    ~SmartArray() {
        delete[] m_Data;
    }

    // kopiowanie glebokie
    SmartArray(const SmartArray& other) {
        ReAlloc(other.m_Capacity);
        for (size_t i = 0; i < other.m_Size; ++i)
            m_Data[i] = other.m_Data[i];
        m_Size = other.m_Size;
    }
    SmartArray& operator=(const SmartArray& other) {
        if (this != &other) {
            delete[] m_Data;
            ReAlloc(other.m_Capacity);
            for (size_t i = 0; i < other.m_Size; ++i)
                m_Data[i] = other.m_Data[i];
            m_Size = other.m_Size;
        }
        return *this;
    }

    // dostepy
    T& operator[](size_t index) { return m_Data[index]; }
    const T& operator[](size_t index) const { return m_Data[index]; }

    T& at(size_t index) {
        if (index >= m_Size) throw std::out_of_range("Index out of range");
        return m_Data[index];
    }
    const T& at(size_t index) const {
        if (index >= m_Size) throw std::out_of_range("Index out of range");
        return m_Data[index];
    }

    // pojemnosc
    bool isEmpty()  const { return m_Size == 0; }
    unsigned size() const { return static_cast<unsigned>(m_Size); }
    unsigned capacity() const { return static_cast<unsigned>(m_Capacity); }

    void reserve(unsigned newCapacity) {
        if (newCapacity > m_Capacity) ReAlloc(newCapacity);
    }

    void resize(unsigned newSize) { resize(newSize, T()); }
    void resize(unsigned newSize, const T& defaultValue) {
        if (newSize > m_Capacity) reserve(newSize);
        for (size_t i = m_Size; i < newSize; ++i)
            m_Data[i] = defaultValue;
        m_Size = newSize;
    }

    void clear() { m_Size = 0; }

    void insert(unsigned index, const T& value) {
        if (index > m_Size) throw std::out_of_range("Index out of range");
        if (m_Size >= m_Capacity) reserve(m_Capacity ? m_Capacity * 2 : 1);
        for (size_t i = m_Size; i > index; --i)
            m_Data[i] = m_Data[i - 1];
        m_Data[index] = value;
        ++m_Size;
    }

    void erase(unsigned index) {
        if (index >= m_Size) throw std::out_of_range("Index out of range");
        for (size_t i = index; i + 1 < m_Size; ++i)
            m_Data[i] = m_Data[i + 1];
        --m_Size;
    }

    void erase(unsigned index, unsigned amount) {
        if (index + amount > m_Size) throw std::out_of_range("Index out of range");
        for (size_t i = index; i + amount < m_Size; ++i)
            m_Data[i] = m_Data[i + amount];
        m_Size -= amount;
    }

    void pushBack(const T& value) {
        if (m_Size >= m_Capacity) ReAlloc(m_Capacity ? m_Capacity * 2 : 1);
        m_Data[m_Size++] = value;
    }

    void popBack() {
        if (m_Size == 0) throw std::out_of_range("Array is empty");
        --m_Size;
    }

    bool SaveToFile(const std::string& filename) const {
        std::ofstream file(filename);
        if (!file.is_open()) return false;
        for (size_t i = 0; i < m_Size; ++i)
            file << m_Data[i].ToCSV() << "\n";
        return true;
    }

    bool LoadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) return false;
        clear();
        std::string line;
        while (std::getline(file, line)) {
            T elem = T::FromCSV(line);
            pushBack(elem);
        }
        return true;
    }

    // wypisywanie
    friend std::ostream& operator<<(std::ostream& os, const SmartArray& arr) {
        os << "[";
        for (size_t i = 0; i < arr.m_Size; ++i) {
            os << arr.m_Data[i];
            if (i + 1 < arr.m_Size) os << ", ";
        }
        os << "]";
        return os;
    }

private:
    void ReAlloc(size_t newCapacity) {
        T* newBlock = new T[newCapacity];
        size_t count = newCapacity < m_Size ? newCapacity : m_Size;
        for (size_t i = 0; i < count; ++i)
            newBlock[i] = m_Data[i];
        delete[] m_Data;
        m_Data = newBlock;
        m_Capacity = newCapacity;
        if (m_Size > newCapacity) m_Size = newCapacity;
    }

    T* m_Data = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
};
