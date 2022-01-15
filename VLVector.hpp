#ifndef EXAMCPP_VLVECTOR_HPP
#define EXAMCPP_VLVECTOR_HPP

#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <iterator>
#include <cstring>
#include "cmath"
#define STATIC_CAPACITY 16


template <class T , __size_t C  = STATIC_CAPACITY> class VLVector
{
private:
    __size_t _size , _capacity;
    T* _members_on_heap;
    T _members_on_stack[C];
    int _mode;
    void posResizeStackToHeap();
    void negResizeHeapToStack();
    void posResizeHeapToHeap();

public:

    VLVector(); // deflt ctor
    VLVector(const VLVector& v1);
    ~VLVector() ;

    template<class InputIterator>
    VLVector(InputIterator first, InputIterator last);
    size_t capacity() const {return _capacity;};
    size_t size() const { return _size;};
    int getMode() const { return _mode;};
    bool empty() const { return !_size; };
    T at(int index);
    T selector(int index);
    void push_back (const T& new_member);
    void clear();
    void pop_back();
    size_t capC() const;
    T* data()  ;
    bool operator==(const VLVector& b) const ;
    bool operator!=(const VLVector& b) const ;

    VLVector& operator=(const VLVector& b);

    T operator[](int index) const;
    bool stackCmpr(const VLVector& b) const ;
    bool heapCmpr(const VLVector& b) const ;


    typedef T value_type;
    typedef std::random_access_iterator_tag iterator_category;
    typedef __size_t difference_type;


    class VLVIterator
    {
    public:
        typedef T* pointer;
        typedef T& reference;
        typedef VLVIterator iterator;

        reference operator*() const { return *_ptr;}
        pointer operator->() const { return  _ptr; }
        VLVIterator():_ptr(nullptr){}

        VLVIterator(const pointer data):_ptr(data){};

        ~VLVIterator(){};
        iterator& operator=(const iterator& other);


        iterator &operator++()
        {
            _ptr++;
            return *this;
        }
        //iterator operator++(T){return _ptr++;}
        iterator &operator--()
        {
            _ptr--;
        return *this;
        }
        //iterator operator--(T){return _ptr--;}

        iterator operator+(difference_type a) const   { return  _ptr + a ;}
        iterator operator-(difference_type a) const   { return  _ptr - a ;}

        difference_type operator-(iterator& other) const {return (other._ptr - _ptr)/ sizeof(pointer) ;}

        iterator &operator+=(difference_type a)
                {
                _ptr +=a;
                return *this;
                }
        iterator &operator-=(difference_type a)
                {
                _ptr -=a;
                return *this;
                }

        bool operator==(const iterator &other)   const  {return _ptr == other._ptr; }
        bool operator!=(const iterator &other)   const  {return _ptr != other._ptr; }
        bool operator<(const iterator &other)    const  {return _ptr < other._ptr;  }
        bool operator>(const iterator &other)    const  {return _ptr > other._ptr;  }
        bool operator<=(const iterator &other)   const  {return _ptr <= other._ptr; }
        bool operator>=(const iterator &other)   const  {return _ptr >= other._ptr; }

    private:
        pointer  _ptr;
    };

    class ConstVLVIterator
    {
    private:

        T*  _ptr;

    public:
        typedef const T* pointer;
        typedef const T& reference;
        typedef ConstVLVIterator const_iterator;

        value_type operator*() const { return *_ptr;}
        reference operator->() const{ return  _ptr;}


        ConstVLVIterator():_ptr(nullptr){}
        explicit ConstVLVIterator(const pointer data):_ptr(data){};
        ~ConstVLVIterator(){}

        const_iterator& operator=(const const_iterator& other);

        const_iterator &operator++()
        {
            _ptr++;
            return *this;
        }
        //const_iterator operator++(T){return _ptr++;}
        const_iterator &operator--(){
            _ptr--;
            return *this;
        }
        //const_iterator operator--(T){return _ptr--;}

        const_iterator &operator+(difference_type a){ return  _ptr + a;}
        const_iterator &operator-(difference_type a){ return  _ptr - a;}

        difference_type operator-(const_iterator& other) const {return (_ptr - other._ptr);}

        const_iterator &operator+=(int a)
                {_ptr +=a;
                 return *this;
                }
        const_iterator &operator-=(int b)
                {
                _ptr +-b;
                return *this;
                }

        bool operator==(const const_iterator& other) const {return _ptr == other._ptr; }
        bool operator!=(const const_iterator& other) const {return _ptr != other._ptr; }
        bool operator<(const const_iterator& other)  const {return _ptr < other._ptr;  }
        bool operator>(const const_iterator& other)  const {return _ptr > other._ptr;  }
        bool operator<=(const const_iterator& other) const {return _ptr <= other._ptr; }
        bool operator>=(const const_iterator& other) const {return _ptr >= other._ptr; }
    };

    typedef VLVIterator iterator;
    iterator begin(){ return iterator(data());}
    iterator end(){ return iterator(data()) += _size;}

    typedef ConstVLVIterator const_iterator;
    const_iterator cbegin() const {return const_iterator(data());}
    const_iterator cend()   const {return const_iterator(data()) += _size;}

    iterator erase(iterator toDelete);
    iterator erase(iterator first , iterator last);

    const_iterator erase(const_iterator toDelete);
    const_iterator erase(const_iterator first , const_iterator last);

    template<class InputIterator>  iterator insert ( InputIterator position , value_type newMember);
    template<class InputIterator> const_iterator insert ( InputIterator position , value_type newMember);

    template<class InputIterator>
    const_iterator insert ( const_iterator position,   InputIterator first,   InputIterator last);

    template<class InputIterator> iterator insert ( iterator position,   InputIterator first,   InputIterator last);

};

template<class T, __size_t C>
size_t VLVector<T, C>::capC() const
{
    return floor(3 * (_size + 1) / 2);
}

template <class T , __size_t C> // deflt ctor
VLVector<T,C> :: VLVector()
:_size(0), _capacity(C),  _mode(0),_members_on_heap(nullptr){}

template <class T , __size_t C> // deflt dtor
VLVector<T,C> :: ~VLVector()
{
    if (_mode)
    {
        delete[] _members_on_heap;
    }
}

template <class T , __size_t C>
void VLVector<T,C> ::posResizeStackToHeap()
{
    __size_t new_capacity = capC();
    T *temp = new T[new_capacity];
    memcpy(temp,_members_on_stack,sizeof(T)*STATIC_CAPACITY);
    _members_on_heap=temp;
    _capacity = new_capacity;
    _mode = 1;
}

template<class T, __size_t C>
void VLVector<T, C>::posResizeHeapToHeap()
{
    __size_t new_capacity = capC();
    T* temp = new T[new_capacity];
    memcpy(temp,_members_on_heap,_capacity* sizeof(T));
    _capacity = new_capacity;
    _members_on_heap=temp;
}

template <class T , __size_t C> // push back
void VLVector<T,C>::push_back(const T &new_member)
{
    if (!_mode)
    {
        if (_size + 1 > C)
        {
            posResizeStackToHeap();
            _members_on_heap[_size]=new_member;
        }
        else
        {
            _members_on_stack[_size]=new_member;
        }
    }

    else if (_mode)
    {
        if (_size +1 > _capacity)
        {
            posResizeHeapToHeap();
        }
        _members_on_heap[_size]=new_member;
    }
    _size++;
}

template <class T , __size_t C>
void VLVector<T,C> ::negResizeHeapToStack()
{
    memcpy(_members_on_stack,_members_on_heap,STATIC_CAPACITY* sizeof(T));
    _mode = 0;
    _capacity = C;
}

template<class T, __size_t C>
void VLVector<T, C>::pop_back()
{
    if (!_mode)
    {
        if (!_size)
        {
            std::cout << "try to pop nothing" << std::endl;
        }
    }
    else if (_mode)
    {
        delete &_members_on_heap[_size - 1];
        if (_size - 1 == C)
        {
            negResizeHeapToStack();
        }
    }
    _size--;
}

template<class T, __size_t C>
void VLVector<T, C>::clear()
{
    unsigned long sizeTotal=_size,i;
    for (i=0; i<sizeTotal; i++)
        pop_back();
}

template<class T, __size_t C>
 T *VLVector<T, C>::data()
{
    if(!_mode)
    {
        T* ptr = _members_on_stack;
        return ptr;
    }
    else if(_mode)
    {
        return _members_on_heap;
    }
}

template<class T, __size_t C>
bool VLVector<T, C>::operator==(const VLVector& b) const
{
    if (_mode != b.getMode() || _size != b.size())
    {
        return false;
    }
    return _mode ? heapCmpr(b):stackCmpr(b);
}

template<class T, __size_t C>
bool VLVector<T, C>::operator!=(const VLVector& b) const
{
    return !(*this == b);
}

template<class T, __size_t C>
bool VLVector<T, C>::stackCmpr(const VLVector &b) const
{

    for (int i=0; i<_size ; i++)
    {
        T t1 = _members_on_stack[i];
        T t2 = b._members_on_stack[i];
        if(t1 != t2)
            return false;
    }
    return true;
}

template<class T, __size_t C>
bool VLVector<T, C>::heapCmpr(const VLVector &b) const
{
    for (int i=0; i<_size ; i++)
    {
        T t1 = _members_on_heap[i];
        T t2 = b._members_on_heap[i];
        if(t1 != t2)
            return false;
    }
    return true;
}

template <class T , __size_t C> // at(i)
T VLVector<T,C> ::at(int index)
{
    if (index < 0 || index > _size || !_size)
    {
        std::cout << "out_of_range" << std::endl;
        exit(EXIT_FAILURE);
    }
    return selector(index);
}

template<class T, __size_t C>
T VLVector<T, C>::selector(int index)
{
    if (!_mode)
    {
        return _members_on_stack[index];
    }
    else if(_mode)
    {
        return _members_on_heap[index];
    }
}

template<class T, __size_t C>
T VLVector<T, C>::operator[](int index) const
{
    return selector(index);
}

template<class T, __size_t C>
VLVector<T, C>::VLVector(const VLVector &v1)
{
    _size = v1._size;
    _mode = v1._mode;
    _capacity =v1._capacity;
    if (_mode)
    {
        memcpy(_members_on_stack,v1._members_on_stack,16* sizeof(T));
    }
    else if (!_mode)
    {
        memcpy(_members_on_heap,v1._members_on_heap,16* sizeof(T));
    }
}

template<class T, __size_t C>
template<class InputIterator>
VLVector<T, C>::VLVector(InputIterator first, InputIterator last)
{
    for ( first; first!=last; ++first)
    {
     push_back(*first);
    }
}

template<class T, __size_t C>
template<class InputIterator>
typename VLVector<T,C>::VLVIterator VLVector<T, C>::insert(InputIterator position,  value_type newMember)
{
    push_back(newMember);
    VLVIterator tail = end();
    VLVIterator curr = end();
    curr-=2;
    tail--;
    while (tail != position -1)
    {
       std::swap((*tail), (*curr));
       curr--,tail--;
    }
}

template<class T, __size_t C>
template<class InputIterator>
typename VLVector<T,C>::VLVIterator VLVector<T, C>::insert(VLVector::VLVIterator position, InputIterator first,
        InputIterator last)
{
    std::for_each(first,last,insert(position,(*first)));
}

template<class T, __size_t C>
template<class InputIterator>
typename VLVector<T,C>::ConstVLVIterator VLVector<T, C>::insert(InputIterator position,  value_type newMember)
{
    push_back(newMember);
    VLVIterator tail = end();
    VLVIterator curr = end();
    curr-=2;
    tail--;
    while (tail != position-1)
    {
        std::swap((*tail), (*curr));
        curr--,tail--;
    }
}

template<class T, __size_t C>
template<class InputIterator>
typename VLVector<T,C>::ConstVLVIterator VLVector<T, C>::insert(VLVector::ConstVLVIterator position,
        InputIterator first, InputIterator last)
{
    std::for_each(first,last,insert(position,(*first)));
}

template<class T, __size_t C>
typename VLVector<T,C>::VLVIterator VLVector<T, C>::erase(VLVector::VLVIterator toDelete)
{
    VLVIterator head = begin();
    VLVIterator curr = begin();
    ConstVLVIterator tail =cend();
    tail--;
    curr++;
    while (head != toDelete)
    {
        curr++, head++;
    }
    while (curr != tail)
    {
        std::swap((*head),(*curr));
        curr++,head++;
    }
    pop_back();
}

template<class T, __size_t C>
typename VLVector<T,C>::ConstVLVIterator VLVector<T, C>::erase(VLVector::ConstVLVIterator toDelete)
{
    VLVIterator head = begin();
    VLVIterator curr = begin();
    ConstVLVIterator tail =cend();
    tail--;
    curr++;
    while (head != toDelete)
    {
        curr++, head++;
    }
    while (curr != tail)
    {
        std::swap((*head),(*curr));
        curr++,head++;
    }
    pop_back();
}


template<class T, __size_t C>
typename VLVector<T,C>::VLVIterator VLVector<T, C>::erase(VLVector::VLVIterator first, VLVector::VLVIterator last)
{
    std::for_each(first, last, erase(*first));
}


template<class T, __size_t C>
typename VLVector<T,C>::ConstVLVIterator VLVector<T, C>::erase(VLVector::ConstVLVIterator first,
                                                               VLVector::ConstVLVIterator last)
{
    std::for_each(first, last, erase(*first));
}

template<class T, __size_t C>
 VLVector<T,C> &VLVector<T, C>::operator=(const VLVector &b)
{
    _mode = b._mode;
    _capacity = b._capacity;
    if (_mode)
    {
        memcpy(_members_on_stack,b._members_on_stack,16* sizeof(T));
    }
    else if (!_mode)
    {
        memcpy(_members_on_heap,b._members_on_heap,16* sizeof(T));
    }
    return *this;
}


#endif //EXAMCPP_VLVECTOR_HPP

