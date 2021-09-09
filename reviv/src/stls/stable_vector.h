#pragma once

#include"rv_pch.hpp"

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

    StableVectorIterator operator++()
    {
        ptr++;
        return *this;
    }

    StableVectorIterator operator++(int)
    {
        StableVectorIterator result = *this;
        ptr++;
        return result;
    }

    bool operator==(const StableVectorIterator& other) const
    {
        return (this->ptr == other.ptr);
    }

    bool operator!=(const StableVectorIterator& other) const
    {
        return !(*this == other);
    }

    ValueType& operator*()
    {
        return *ptr;
    }

    ValueType* operator->()
    {
        return ptr;
    }

/*
    // not tested
    StableVectorIterator operator+(int offset)
    {
        StableVectorIterator result;
        result.ptr = ptr + offset;
    }

    // not tested
    StableVectorIterator operator-(int offset)
    {
        StableVectorIterator result;
        result.ptr = ptr - offset;
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

        StableVector();
        StableVector(unsigned int capacity);
        StableVector(std::vector<T> inVector);
        StableVector(const StableVector&) = delete;
        ~StableVector();

        StableVector& operator=(const StableVector &other);
        T& operator [] (int index);
        const T& operator [] (int index) const;
        
        void pushBack(const T& element);

        template<class ...Args>
        void emplaceBack(Args&&... args);

        unsigned int size() const;
        void reserve(unsigned int size);
        T& back();

        void clear();

        Iterator begin() const
        {
            return Iterator((T*)ptr);
        }
        Iterator end() const
        {
            return Iterator((T*)ptr + currentSize);
        }

    unsigned int capacity=0;
    unsigned int currentSize;

    private:
        void* ptr = nullptr;
//        Iterator m_begin;
//        Iterator m_end;
    };

}

namespace stls
{
    template<class T>
    StableVector<T>::StableVector()
        : capacity(0), currentSize(0)
    { }

    template<class T>
    StableVector<T>::StableVector(unsigned int capacity)
        : capacity(capacity), currentSize(0)
    {
        ptr = new T[capacity]();
    }

    template<class T>
    StableVector<T>::StableVector(std::vector<T> inVector)
    {
        reserve(inVector.capacity());
        currentSize = inVector.size();

        for(int i=0; i<inVector.size(); i++)
        {
            (*this)[i] = inVector[i];
        }
    }

    template<class T>
    StableVector<T>::~StableVector()
    {
        delete[] (T*)ptr;
    }

    template<class T>
    T& StableVector<T>::operator [] (int index)
    {
        assert(index < currentSize && index >= 0);
        return *((T*)ptr + index);
    }

    template<class T>
    const T& StableVector<T>::operator [] (int index) const
    {
        assert(index < currentSize && index >= 0);
        return *((T*)ptr + index);
    }

    template<class T>
    StableVector<T>& StableVector<T>::operator=(const StableVector &other)
    {
        assert(this->capacity == 0 && this->ptr == nullptr);
        this->reserve(other.capacity); 
        this->currentSize = other.currentSize;

        for(int i=0; i < other.size(); i++)
        {
            (*this)[i] = other[i];
        }

        return *this;
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

    template<class T>
    void StableVector<T>::clear()
    {
        for(int i=0; i<currentSize; i++)
        {
            (*this)[i].~T();
        }
        currentSize = 0;
    }

    template<class T>
    void StableVector<T>::reserve(unsigned int size)
    {
        assert(capacity == 0 && ptr == nullptr); // reserve can be used only once on StableVector
        capacity = size;
        ptr = new T[capacity]();
    }

    template<class T>
    T& StableVector<T>::back()
    {
        return (*this)[currentSize - 1];
    }
}