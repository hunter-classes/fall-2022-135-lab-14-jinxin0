template<class MyVector>
class MyVectorIterator
{
public:
    using ValueType = typename MyVector::ValueType;

private:
    ValueType* m_Ptr;

public:
    MyVectorIterator(ValueType* ptr)
        : m_Ptr(ptr)
    {
    }

    ~MyVectorIterator()
    {
    }

    MyVectorIterator& operator++()
    {
        m_Ptr++;
        return *this;
    }

    MyVectorIterator operator++(int)
    {
        MyVectorIterator temp = *this; // save current state
        m_Ptr++; // increment pointer
        return temp; // return the old state
    }

    MyVectorIterator& operator--()
    {
        m_Ptr--;
        return *this;
    }

    MyVectorIterator operator--(int)
    {
        MyVectorIterator temp = *this; // save current state
        m_Ptr--; // decrement pointer
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

    bool operator==(const MyVectorIterator& other) const
    {
        return m_Ptr == other.m_Ptr;
    }

    bool operator!=(const MyVectorIterator& other) const
    {
        return m_Ptr != other.m_Ptr;
    }
};

template <typename T>
class MyVector
{
private:
    std::size_t m_Size = 0;
    std::size_t m_Capacity;
    T* m_Array = nullptr;

public:
    using ValueType = T;
    using Iterator = MyVectorIterator<MyVector<T>>;

private:
    void copy(const T* const from, T* const to, std::size_t count)
    {
        for (std::size_t i = 0; i < count; i++)
            *(to + i) = *(from + i);
    }

    void move(T* const from, T* const to, std::size_t count)
    {
        for (std::size_t i = 0; i < count; i++)
            *(to + i) = std::move(*(from + i));
    }

public:
    MyVector(const std::size_t& capacity = 2)
        : m_Capacity(capacity),
        m_Array((T*)::operator new(capacity * sizeof(T)))
    {
    }

    MyVector(const MyVector<T>& array)
        : m_Size(array.size()),
        m_Capacity(array.capcity()),
        m_Array((T*)::operator new(array.capcity() * sizeof(T)))
    {
        copy(array.data(), data(), std::min(array.size(), size()));
    }

    MyVector(MyVector<T>&& array)
        : m_Capacity(std::move(array.capcity())),
        m_Size(std::move(array.size())),
        m_Array(std::move(array.data()))
    {
        array.m_Array = nullptr;
    }

    ~MyVector()
    {
        delete[] m_Array;
    }

    T& operator[](const std::size_t& index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bound");

        return data()[index];    
    }

    const T& at(const std::size_t& index) const
    {
        if (index >= size())
            throw std::out_of_range("Index out of bound");

        return m_Array[index];
    }

    T& at(const std::size_t& index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bound");

        return data()[index];
    }

    void resize(const std::size_t& capacity)
    {
        if (capcity() == capacity)
            return;
        
        m_Capacity = capacity;
        if (size() > capacity)
            m_Size = capacity;
        T* newArray = (T*)::operator new(capacity * sizeof(T));
        move(data(), newArray, size());
        delete[] m_Array;
        m_Array = newArray;
    }

    // shrink capacity to size
    void shrinkToFit()
    {
        resize(size());
    }

    void softClear()
    {
        m_Size = 0;
    }

    void hardClear()
    {
        resize(0);
    }
    
    void clear()
    {
        softClear();
    }

    // remove element at index then shift items in array down
    void remove(const std::size_t& index)
    {
        if (index >= size())
            throw std::out_of_range("Index out of bound");

        // shifting left
        for (std::size_t i = index, end = size() - 1; i < end; ++i)
            at(i) = at(i + 1);
        --m_Size;
    }

    void insert(const std::size_t& index, const T& item)
    {
        if (index > size())
            throw std::out_of_range("Index out of bound");

        if (size() == capcity())
            resize(capcity() * 1.5);

        // shifting right
        for (std::size_t i = size(); i > index; --i)
            m_Array[i] = at(i - 1);
        at(index) = item;
        ++m_Size;
    }
    
    void insert(const std::size_t& index, T&& item)
    {
        if (index > size())
            throw std::out_of_range("Index out of bound");

        if (size() == capcity())
            resize(capcity() * 1.5);

        // shifting right
        for (std::size_t i = size(); i > index; --i)
            m_Array[i] = at(i - 1);
        at(index) = std::move(item);
        ++m_Size;
    }

    void pop_back()
    {
        --m_Size;
    }

    void push_back(const T& item)
    {
        if (size() >= capcity())
            resize(capcity() * 1.5);
        at(m_Size++) = item;
    }
    
    void push_back(T&& item)
    {
        if (size() >= capcity())
            resize(capcity() * 1.5);
        at(m_Size++) = std::move(item);
    }

    template<typename... Args>
    void emplace_back(Args&&... args)
    {
        if (size() >= capcity())
            resize(capcity() * 1.5);
        new(&m_Array[size()]) T(std::forward<Args>(args)...);
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
        return Iterator(data() + size());
    }

    const Iterator begin() const 
    {
        return Iterator(data());
    }

    const Iterator end() const
    {
        return Iterator(data() + size());
    }
    
    const Iterator cbegin() const 
    {
        return Iterator(data());
    }

    const Iterator cend() const
    {
        return Iterator(data() + size());
    }

    T& front()
    {
        return at(0);
    }

    T& back()
    {
        return at(size() - 1);
    }

    bool empty() const
    {
        return (size() == 0);
    }

    std::size_t size() const
    {
        return m_Size;
    }

    std::size_t capcity() const
    {
        return m_Capacity;
    }
};
