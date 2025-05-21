#pragma once

template<typename T>
class SmartArray {
public:
    SmartArray() {
        ReAlloc(2);
    }

    ~SmartArray() {
        delete[] m_Data;
    }

    SmartArray(const SmartArray& other) {
        ReAlloc(other.m_Capacity);
        for (size_t i = 0; i < other.m_Size; i++)
            m_Data[i] = other.m_Data[i];
        m_Size = other.m_Size;
    }

    SmartArray& operator=(const SmartArray& other) {
        if (this != &other) {
            delete[] m_Data;
            ReAlloc(other.m_Capacity);
            for (size_t i = 0; i < other.m_Size; i++)
                m_Data[i] = other.m_Data[i];
            m_Size = other.m_Size;
        }
        return *this;
    }

    void PushBack(const T& value) {
        if (m_Size >= m_Capacity)
            ReAlloc(m_Capacity * 2);
        m_Data[m_Size] = value;
        m_Size++;
    }

    void RemoveElem(size_t index){
        if (index >= m_Size) return;
        for (size_t i = index; i < m_Size - 1; ++i)
            m_Data[i] = m_Data[i + 1];
        --m_Size;

    }

    void EditElem(size_t index, const T& newValue) {
        if (index >= m_Size) return;
        m_Data[index] = newValue;
    }
    void Clear() {
        m_Size = 0;
    }
    size_t Size() const { return m_Size; }
    size_t Capacity() const { return m_Capacity; }

    T& operator[](size_t index) { return m_Data[index]; }
    const T& operator[](size_t index) const { return m_Data[index]; }

private:
    void ReAlloc(size_t newCapacity) {
        T* newBlock = new T[newCapacity];
        if (newCapacity < m_Size)
            m_Size = newCapacity;

        for (size_t i = 0; i < m_Size; i++)
            newBlock[i] = m_Data[i];

        delete[] m_Data;
        m_Data = newBlock;
        m_Capacity = newCapacity;
    }

private:
    T* m_Data = nullptr;
    size_t m_Size = 0;
    size_t m_Capacity = 0;
};