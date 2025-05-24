#pragma once
#include <stdexcept>
#include <ostream>
#include <cstddef>
#include <fstream>

template<typename T = int>
class SmartArray {
public:
    SmartArray() {
        ReAlloc(2);
    }

    explicit SmartArray(size_t size) {
        ReAlloc(size);
        for (size_t i = 0; i < size; ++i)
            m_Data[i] = T();
        m_Size = size;
    }

    SmartArray(size_t size, const T& defaultValue) {
        ReAlloc(size);
        for (size_t i = 0; i < size; ++i)
            m_Data[i] = defaultValue;
        m_Size = size;
    }

    ~SmartArray() {
        delete[] m_Data;
    }

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

    //   operacje na Smartarray
    void PushBack(const T& value) {
        if (m_Size >= m_Capacity)
            ReAlloc(m_Capacity ? m_Capacity * 2 : 1);
        m_Data[m_Size++] = value;
    }

    void RemoveElem(size_t index) {
        if (index >= m_Size) return;
        for (size_t i = index; i + 1 < m_Size; ++i)
            m_Data[i] = m_Data[i + 1];
        --m_Size;
    }

    void EditElem(size_t index, const T& newValue) {
        if (index < m_Size)
            m_Data[index] = newValue;
    }

    void Clear() { m_Size = 0; }

    //       Capacity 
    size_t Size() const { return m_Size; }
    size_t Capacity() const { return m_Capacity; }
    bool IsEmpty() const { return m_Size == 0; }

    //     Dostepowe rzeczy
    T& At(size_t index) {
        if (index >= m_Size)
            throw std::out_of_range("Index out of range");
        return m_Data[index];
    }
    const T& At(size_t index) const {
        if (index >= m_Size)
            throw std::out_of_range("Index out of range");
        return m_Data[index];
    }
    T& operator[](size_t index) { return m_Data[index]; }
    const T& operator[](size_t index) const { return m_Data[index]; }

    //      Capacity  
    void Reserve(size_t newCapacity) {
        if (newCapacity > m_Capacity)
            ReAlloc(newCapacity);
    }
    void Resize(size_t newSize) { Resize(newSize, T()); }
    void Resize(size_t newSize, const T& defaultValue) {
        if (newSize > m_Capacity)
            Reserve(newSize);
        for (size_t i = m_Size; i < newSize; ++i)
            m_Data[i] = defaultValue;
        m_Size = newSize;
    }
    void ShrinkToFit() { ReAlloc(m_Size); }

    //      Iteracja
    void Insert(size_t index, const T& value) {
        if (index > m_Size) return;
        if (m_Size >= m_Capacity)
            Reserve(m_Capacity ? m_Capacity * 2 : 1);
        for (size_t i = m_Size; i > index; --i)
            m_Data[i] = m_Data[i - 1];
        m_Data[index] = value;
        ++m_Size;
    }
    void PushFront(const T& value) { Insert(0, value); }
    void PopBack()  { if (m_Size) --m_Size; }
    void PopFront() { RemoveElem(0); }

    //      Porownywanie
    bool operator==(const SmartArray& other) const {
        if (m_Size != other.m_Size) return false;
        for (size_t i = 0; i < m_Size; ++i)
            if (!(m_Data[i] == other.m_Data[i]))
                return false;
        return true;
    }

    friend std::ostream& operator<<(std::ostream& os, const SmartArray& arr) {
        os << "[";
        for (size_t i = 0; i < arr.m_Size; ++i) {
            os << arr.m_Data[i];
            if (i + 1 < arr.m_Size)
                os << ", ";
        }
        os << "]";
        return os;
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
        Clear();
        std::string line;
        while (std::getline(file, line)) {
            Employee e = Employee::FromCSV(line);
            PushBack(e);
        }
        return true;
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
        if (m_Size > newCapacity)
            m_Size = newCapacity;
    }

    T* m_Data      = nullptr;
    size_t m_Size  = 0;
    size_t m_Capacity = 0;
};