template<class DynamicArray>
class DynamicArrayIterator
{
public:
    using ValueType = typename DynamicArray::ValueType;

private:
    ValueType* m_Ptr;

public:
    DynamicArrayIterator(ValueType* ptr)
        : m_Ptr(ptr)
    {
    }

    ~DynamicArrayIterator()
    {
    }

    DynamicArrayIterator& operator++()
    {
        m_Ptr++;
        return *this;
    }

    DynamicArrayIterator operator++(int)
    {
        DynamicArrayIterator temp = *this; // save current state
        m_Ptr++; // increment pointer
        return temp; // return the old state
    }

    DynamicArrayIterator& operator--()
    {
        m_Ptr--;
        return *this;
    }

    DynamicArrayIterator operator--(int)
    {
        DynamicArrayIterator temp = *this; // save current state
        m_Ptr--; // increment pointer
        return temp; // return the old state
    }

    ValueType& operator[](int index)
    {
        return m_Ptr[index];
    }

    ValueType* operator->()
    {
        return m_Ptr;
    }

    ValueType& operator*()
    {
        return *(m_Ptr);
    }

    bool operator==(const DynamicArrayIterator& other) const
    {
        return m_Ptr == other.m_Ptr;
    }

    bool operator!=(const DynamicArrayIterator& other) const
    {
        return m_Ptr != other.m_Ptr;
    }
};

template <typename T>
class DynamicArray
{
private:
    std::size_t m_Size = 0;
    std::size_t m_Capacity;
    T* m_Array = nullptr;

public:
    using ValueType = T;
    using Iterator = DynamicArrayIterator<DynamicArray<T>>;

private:
    void copy(T* const from, T* const to, std::size_t count)
    {
        for (std::size_t i = 0; i < count; i++)
            to[i] = from[i];
    }

    void move(T* const from, T* const to, std::size_t count)
    {
        for (std::size_t i = 0; i < count; i++)
            to[i] = std::move(from[i]);
    }

public:
    DynamicArray(const std::size_t& capacity = 2)
        : m_Capacity(capacity),
        m_Array((T*)::operator new(capacity * sizeof(T)))
    {
    }

    DynamicArray(const DynamicArray<T>& array)
        : m_Size(array.getSize()),
        m_Capacity(array.getCapacity()),
        m_Array((T*)::operator new(array.getCapacity() * sizeof(T)))
    {
        copy(array.data(), data(), std::min(array.getSize(), getSize()));
    }

    DynamicArray(DynamicArray<T>&& array)
        : m_Capacity(std::move(array.getCapacity())),
        m_Size(std::move(array.getSize())),
        m_Array(std::move(array.data()))
    {
        array.m_Array = nullptr;
    }

    ~DynamicArray()
    {
        delete[] m_Array;
    }

    T& operator[](const std::size_t& index)
    {
        if (index >= getSize())
            throw std::out_of_range("Index out of bound");

        return data()[index];    
    }

    const T& at(const std::size_t& index) const
    {
        if (index >= getSize())
            throw std::out_of_range("Index out of bound");

        return m_Array[index];
    }

    T& at(const std::size_t& index)
    {
        if (index >= getSize())
            throw std::out_of_range("Index out of bound");

        return data()[index];
    }

    void resize(const std::size_t& capacity)
    {
        if (getCapacity() == capacity)
            return;
        
        m_Capacity = capacity;
        if (getSize() > capacity)
            m_Size = capacity;
        T* newArray = (T*)::operator new(capacity * sizeof(T));
        move(data(), newArray, getSize());
        delete[] m_Array;
        m_Array = newArray;
    }

    // shrink capacity to size
    void shrinkToFit()
    {
        resize(getSize());
    }

    void softClear()
    {
        m_Size = 0;
    }

    void hardClear()
    {
        resize(0);
    }
    
    // remove element at index then shift items in array down
    void remove(const std::size_t& index)
    {
        if (index >= getSize())
            throw std::out_of_range("Index out of bound");

        // shifting left
        for (std::size_t i = index, end = getSize() - 1; i < end; ++i)
            at(i) = at(i + 1);
        --m_Size;
    }

    void insert(const std::size_t& index, const T& item)
    {
        if (index > getSize())
            throw std::out_of_range("Index out of bound");

        if (getSize() == getCapacity())
            resize(getCapacity() * 1.5);

        // shifting right
        for (std::size_t i = getSize(); i > index; --i)
            m_Array[i] = at(i - 1);
        at(index) = item;
        ++m_Size;
    }
    
    void insert(const std::size_t& index, T&& item)
    {
        if (index > getSize())
            throw std::out_of_range("Index out of bound");

        if (getSize() == getCapacity())
            resize(getCapacity() * 1.5);

        // shifting right
        for (std::size_t i = getSize(); i > index; --i)
            m_Array[i] = at(i - 1);
        at(index) = std::move(item);
        ++m_Size;
    }

    void popBack()
    {
        --m_Size;
    }

    void pushBack(const T& item)
    {
        if (getSize() >= getCapacity())
            resize(getCapacity() * 1.5);
        at(m_Size++) = item;
    }
    
    void pushBack(T&& item)
    {
        if (getSize() >= getCapacity())
            resize(getCapacity() * 1.5);
        at(m_Size++) = std::move(item);
    }

    template<typename... Args>
    void emplaceBack(Args&&... args)
    {
        if (getSize() >= getCapacity())
            resize(getCapacity() * 1.5);
        new(&m_Array[getSize()]) T(std::forward<Args>(args)...);
        ++m_Size;
    }
    
    // pointer to the array that is storing the data
    T* data()
    {
        return m_Array;
    }

    T* const data() const
    {
        return m_Array;
    }

    Iterator begin()
    {
        return Iterator(data());
    }

    Iterator end()
    {
        return Iterator(data() + getSize());
    }
    
    const Iterator begin() const 
    {
        return Iterator(data());
    }

    const Iterator end() const
    {
        return Iterator(data() + getSize());
    }

    T& front()
    {
        return at(0);
    }

    T& back()
    {
        return at(getSize() - 1);
    }

    bool isEmpty() const
    {
        return (getSize() == 0);
    }

    std::size_t getSize() const
    {
        return m_Size;
    }

    std::size_t getCapacity() const
    {
        return m_Capacity;
    }
};
