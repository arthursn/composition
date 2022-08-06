/// @file src/iterator.h

#ifndef ITERATOR_H
#define ITERATOR_H

#include <cstddef> // For std::ptrdiff_t
#include <iterator> // For std::forward_iterator_tag

/** @brief A custom of iterator
 * 
 * See https://www.internalpointers.com/post/writing-custom-iterators-modern-cpp
 * 
 * Say you have the following structure:
 * 
 * @code{.cpp}
 * struct Base {
 *     int mA;
 * };
 * 
 * struct Derived : public Base {
 *     int mB;
 * 
 *     Derived(int a, int b)
 *         : Base { a }
 *         , mB(b)
 *     {
 *     }
 * };
 * 
 * Derived myContainer[200] = { Derived(1, 2), Derived(3, 4), ... };
 * @endcode
 * 
 * You can use Iterator to iterate over `myContainer`:
 * 
 * @code{.cpp}
 * Iterator<Derived> begin(&myContainer[0]);
 * Iterator<Derived> end(&myContainer[200]);
 * for (Iterator<Derived> it = begin; it != end; it++) {
 *      std::cout << it->mA << std::endl;
 *      std::cout << it->mB << std::endl;
 * }
 * @endcode
 *  
 * Iterator has a cool feature: you can iterate over a container of objects sliced
 * to their base classes. So, using the structure from the example above, you
 * could do the following:
 * 
 * @code{.cpp}
 * Iterator<Base> begin(&myContainer[0], sizeof(Derived));
 * Iterator<Base> end(&myContainer[200], sizeof(Derived));
 * for (Iterator<Base> it = begin; it != end; it++) {
 *       std::cout << it->mA << std::endl;
 *       // std::cout << it->mB << std::endl; // This fails
 * }
 * @endcode
 */
template <typename T>
class Iterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    Iterator(pointer ptr, int size = -1)
        : mvpPtr(ptr)
        , mvSize(size < 0 ? sizeof(T) : size)
    {
    }

    reference operator*() const { return *mvpPtr; }
    pointer operator->() { return mvpPtr; }
    Iterator& operator++()
    {
        // This is a trick to be able to iterate over containers of objects sliced to their base classes.
        mvpPtr = (pointer)((char*)mvpPtr + mvSize);
        return *this;
    }
    Iterator operator++(int)
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }
    friend bool operator==(const Iterator& a, const Iterator& b) { return a.mvpPtr == b.mvpPtr; };
    friend bool operator!=(const Iterator& a, const Iterator& b) { return a.mvpPtr != b.mvpPtr; };

private:
    pointer mvpPtr; ///< Pointer to element of the container
    size_t mvSize; ///< Size of the element of the container (before casting)
};

/** @brief Another type of iterator for a container of pointers.
 * 
 * Motivation: Let's say you have the following container:
 * 
 * @code{.cpp}
 * int a = 42;
 * int b = 69;
 * int* myContainer[200] = { &a, &b, ... };
 * @endcode
 *
 * One can create an iterator with #Iterator:
 * 
 * @code 
 * Iterator<int*> begin(&myContainer[0]);
 * Iterator<int*> end(&myContainer[200]);
 * for (Iterator<int*> it = begin; it != end; it++) {
 *     std::cout << **it << std::endl;
 * }
 * @endcode
 * 
 * But notice that when looping through the container, you have to
 * dereference the iterator twice. With PointersIterator you can 
 * avoid that:
 * 
 * @code{.cpp}
 * PointersIterator<int> begin(&myContainer[0]);
 * PointersIterator<int> end(&myContainer[200]);
 * for (PointersIterator<int> it = begin; it != end; it++) {
 *     std::cout << *it << std::endl;
 * }
 * @endcode
 */
template <typename T>
class PointersIterator {
public:
    using iterator_category = std::forward_iterator_tag;
    using difference_type = std::ptrdiff_t;
    using value_type = T;
    using pointer = T*;
    using reference = T&;

    PointersIterator(pointer* ptrPtr)
        : mvpPtrPtr(ptrPtr)
    {
    }

    reference operator*() const { return **mvpPtrPtr; }
    pointer operator->() { return *mvpPtrPtr; }
    PointersIterator& operator++()
    {
        ++mvpPtrPtr;
        return *this;
    }
    PointersIterator operator++(int)
    {
        PointersIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    friend bool operator==(const PointersIterator& a, const PointersIterator& b) { return a.mvpPtrPtr == b.mvpPtrPtr; };
    friend bool operator!=(const PointersIterator& a, const PointersIterator& b) { return a.mvpPtrPtr != b.mvpPtrPtr; };

private:
    pointer* mvpPtrPtr; ///< Pointer to element of the container (which is also a pointer!)
};

#endif