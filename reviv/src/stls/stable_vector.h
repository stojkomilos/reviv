#pragma once

#include<assert.h>
#include<iostream>
#include<new>

using std::cin; using std::cout; using std::endl;

template<class StableVector>
class StableVectorIterator
{
public:
    using ValueType = typename StableVector::ValueType;

    StableVectorIterator(ValueType* ptr)
        : ptr(ptr)
    {
    }

    StableVectorIterator operator++(int)
    {
        StableVectorIterator result = *this;
        ptr++;
        return result;
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

    ValueType* operator->()
    {
        return ptr;
    }

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
        StableVector(const StableVector&) = default; //TODO: delete
        StableVector& operator=(const StableVector &t) = default; //TODO: delete
        ~StableVector();
        T& operator [] (int index);
        const T& operator [] (int index) const;
        
        void pushBack(const T& element);

        template<class ...Args>
        void emplaceBack(Args&&... args);

        unsigned int size() const;

        Iterator begin() const
        {
            return Iterator((T*)ptr);
        }
        Iterator end() const
        {
            return Iterator((T*)ptr + currentSize); //TODO, da li ovo valja?
        }

    unsigned int capacity;
    unsigned int currentSize;

    private:
        void* ptr;
//        Iterator m_begin;
//        Iterator m_end;
    };

}

namespace stls
{

    template<class T>
    StableVector<T>::StableVector(unsigned int capacity)
        : capacity(capacity), currentSize(0)
    {
        ptr = new T[capacity]();
    }

    template<class T>
    StableVector<T>::~StableVector()
    {
        delete[] (T*)ptr;
    }

    template<class T>
    T& StableVector<T>::operator [] (int index)
    {
        assert(index < currentSize and index >= 0);
        return *((T*)ptr + index);
    }

    template<class T>
    const T& StableVector<T>::operator [] (int index) const
    {
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

    template <class T>
    template <class... Args>
    void StableVector<T>::emplaceBack(Args&&... args)
    {
        currentSize++;
        assert(currentSize <= capacity);
        new((T*)(&(*this)[currentSize-1])) T(std::forward<Args>(args)...);
        //*(T*)(&(*this)[currentSize-1])(std::forward<Args>(args)...);


    //    new(&(*this)[currentSize-1]) T(std::forward<Args>(args)...); // NOTE: does not require a "delete", because this is "placement new"
    }


    template<class T>
    unsigned int StableVector<T>::size() const
    {
        return currentSize;
    }
}