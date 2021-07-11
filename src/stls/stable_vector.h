#include<assert.h>
#include<iostream>
using std::cin; using std::cout; using std::endl;


template<class StableVector>
class StableVectorIterator
{
public:
    using ValueType = typename StableVector::ValueType;
    //using PointerType = typename StableVector::ValueType*;

    StableVectorIterator(ValueType* ptr)
        : ptr(ptr)
    {
    }

    StableVectorIterator& operator++()
    {
        ptr++;
        return *this;
    }

    bool operator==(const StableVectorIterator& other) const
    {
        return this->ptr == other.ptr;
    }

    bool operator!=(const StableVectorIterator& other) const
    {
        return !(*this == other);
    }

    ValueType operator*()
    {
        return *ptr;
    }

/*
    StableVector& operator--()
    {
        ptr--
        return *this;
    }

    ValueType& operator[](int index)
    {
        return *(ptr + index);
    }

    ValueType* operator->()
    {
        return ptr;
    }

    ValueType* operator*()
    {
        return *ptr;
    }

    bool operator==(const StableVectorIterator& other) const
    {
        return ptr == other.ptr;
    }

    bool operator==(const StableVectorIterator& other) const
    {
        return !(*this == other)
    }
    */

private:
    ValueType* ptr;
};



namespace stls
{
    template <class T>
    class StableVector
    {
    public:
        using ValueType = T;
        using Iterator = StableVectorIterator<StableVector<T>>;

        StableVector(unsigned int capacity);

        StableVector() = delete;
        StableVector(const StableVector&) = delete;
        StableVector& operator = (const StableVector &t) = delete;
        ~StableVector();
        T& operator [] (int index);
        const T& operator [] (int index) const;
        
        void pushBack(const T& element);

        template<class ...Args>
        T& emplaceBack(Args&&... args);

        unsigned int size();

        Iterator begin()
        {
            return Iterator((T*)ptr);
        }
        Iterator end()
        {
            return Iterator((T*)ptr + currentSize); //TODO, da li ovo valja?
        }

    private:
        unsigned int capacity;
        unsigned int currentSize;
        void* ptr;
    };

}

namespace stls
{

    template<class T>
    StableVector<T>::StableVector(unsigned int capacity)
        : capacity(capacity)
    {
        ptr = new T[capacity];
    }

    template<class T>
    StableVector<T>::~StableVector()
    {
        delete[] (T*)ptr;
    }

    template<class T>
    T& StableVector<T>::operator [] (int index)
    {
        //cout << "NON-const index=" << index << endl;
        assert(index < currentSize and index >= 0);
        return *((T*)ptr + index);
    }

    template<class T>
    const T& StableVector<T>::operator [] (int index) const
    {
        cout << "const index=" << index << endl;
        assert(index < currentSize and index >= 0);
        return *((T*)ptr + index);
    }

    template<class T>
    void StableVector<T>::pushBack(const T& element)
    {
        currentSize++;
        assert(currentSize <= capacity);
        (*this)[currentSize-1] = element;
    }

    template<class T>
    template<class ...Args>
    T& StableVector<T>::emplaceBack(Args&&... args)
    {
        currentSize++;
        assert(currentSize <= capacity);
        return (*this)[currentSize-1] = T(std::forward<Args>(args)...);
    }

    template<class T>
    unsigned int StableVector<T>::size()
    {
        return currentSize;
    }
}