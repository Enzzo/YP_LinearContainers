#pragma once

#include <cassert>
#include <initializer_list>
#include <algorithm>
#include <stdexcept>

#include "array_ptr.h"

class ReserveProxyObj {
    size_t size_;
public:
    ReserveProxyObj(size_t size = 0) : size_(size) {}
    inline size_t GetSize() const {
        return size_;
    }
};

ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {

    ArrayPtr<Type> items_;
    size_t size_ = 0;
    size_t capacity_ = 0;

public:
    using Iterator = Type*;
    using ConstIterator = const Type*;

    SimpleVector() noexcept = default;

    explicit SimpleVector(size_t size) : SimpleVector(size, Type()) {}

    SimpleVector(size_t size, const Type& value) :size_(size), capacity_(size) {
        if (size_ > 0) {            
            ArrayPtr<Type> temp(size_);            
            Iterator first = temp.Get();
            Iterator last = temp.Get() + size_;
            std::fill(first, last, value);
            items_.swap(temp);            
        }
    }

    SimpleVector(ReserveProxyObj capacity) {
        Reserve(capacity.GetSize());
    }

    SimpleVector(std::initializer_list<Type> init) :size_(init.size()), capacity_(init.size()) {
        if (size_ > 0) {
            ArrayPtr<Type> temp(size_);
            std::copy(init.begin(), init.end(), temp.Get());
            items_.swap(temp);
        }
    }

    SimpleVector(const SimpleVector& other) {        
        SimpleVector<Type> temp(other.GetSize());
        std::copy(other.begin(), other.end(), temp.begin());
        temp.size_ = other.size_;
        temp.capacity_ = other.capacity_;
        swap(temp);        
    }

    SimpleVector(SimpleVector&& other) {        
        SimpleVector temp;
        temp.items_.swap(other.items_);
        temp.size_ = std::exchange(other.size_, 0);
        temp.capacity_ = std::exchange(other.capacity_, 0);
        swap(temp);        
    }
    
    SimpleVector& operator=(const SimpleVector& rhs) {        
        assert(*this != rhs);
        SimpleVector temp(rhs);
        swap(temp);        
        return *this;
    }
    
    SimpleVector& operator=(SimpleVector&& rhs) {        
        assert(*this != rhs);
        this->Resize(rhs.GetSize());
        std::move(rhs.begin(), rhs.end(), begin());
        return *this;
    }
    
    void PushBack(const Type& item);
    
    void PushBack(Type&& item);

    void PopBack() noexcept;

    Iterator Insert(ConstIterator pos, Type&& value) {

        if (begin() == end()) {
            Resize(size_ + 1);
            *begin() = std::move(value);
            return begin();
        }

        Iterator p = const_cast<Iterator>(pos);
        int dist = std::distance(p, end());
        ArrayPtr<Type> temp(dist);
        std::move(p, end(), temp.Get());
        Resize(size_ + 1);        
        Iterator t = end() - dist - 1; //target iterator
        *(t) = std::move(value);
        std::move(temp.Get(), temp.Get() + dist, t+1);
        return t;
    }   

    Iterator Erase(ConstIterator pos) {
        assert(size_ > 0);

        Iterator first = const_cast<Iterator>(pos + 1);
        Iterator p = const_cast<Iterator>(pos);
        Iterator last = end();
        --size_;
        std::move(first, last, p);

        return p;
    }

    void swap(SimpleVector& other) noexcept;

    inline size_t GetSize() const noexcept {
        return size_;
    }

    inline size_t GetCapacity() const noexcept {
        return capacity_;
    }

    inline bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    inline void Clear()  noexcept {
        size_ = 0;
    }

    void Reserve(size_t new_capacity);

    void Resize(size_t new_size) noexcept;

    inline Type& operator[](size_t index) noexcept {
        return items_[index];
    }

    inline const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    Type& At(size_t index);

    const Type& At(size_t index) const;

    Iterator begin() noexcept {
        return items_.Get();
    }

    Iterator end() noexcept {
        return items_.Get() + size_;
    }

    ConstIterator begin() const noexcept {
        return items_.Get();
    }

    ConstIterator end() const noexcept {
        return items_.Get() + size_;
    }

    ConstIterator cbegin() const noexcept {
        return items_.Get();
    }

    ConstIterator cend() const noexcept {
        return items_.Get() + size_;
    }
};

template<typename Type>
void SimpleVector<Type>::PushBack(const Type& item) {
    Resize(size_ + 1);
    *(end() - 1) = item;
}

template<typename Type>
void SimpleVector<Type>::PushBack(Type&& item) {
    Resize(size_ + 1);
    *(end() - 1) = std::move(item);
}

template<typename Type>
void SimpleVector<Type>::PopBack() noexcept {
    assert(size_ > 0);
    --size_;
}

template<typename Type>
void SimpleVector<Type>::swap(SimpleVector& other) noexcept {
    items_.swap(other.items_);
    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

template<typename Type>
void SimpleVector<Type>::Reserve(size_t new_capacity) {
    if (new_capacity > capacity_) {
        ArrayPtr<Type> temp(new_capacity);
        if (begin() != end()) {
            std::move(items_.Get(), items_.Get() + capacity_, temp.Get());
        }
        Iterator first = temp.Get() + capacity_;
        Iterator last = temp.Get() + new_capacity;   
        
        for (Iterator it = first; it != last; ++it) {
            *it = Type();
        }
        items_.swap(temp);
        capacity_ = new_capacity;
    }
}

template<typename Type>
void SimpleVector<Type>::Resize(size_t new_size) noexcept {
    if (new_size > size_) {

        if (new_size > capacity_) {
            while (new_size > capacity_) {
                capacity_ = (capacity_ == 0) ? 1 : capacity_ * 2;
            }
            ArrayPtr<Type> temp(capacity_);
            if (begin() != nullptr) {
                std::move(begin(), end(), temp.Get());
            }
            items_.swap(temp);
        }
        Iterator first = begin() + size_;
        Iterator last = begin() + new_size;

        for (auto it = first; it != last; ++it) {
            *it = Type();
        }
    }
    size_ = new_size;
}

template<typename Type>
Type& SimpleVector<Type>::At(size_t index) {
    if (index >= size_) {
        throw std::out_of_range("");
    }
    return items_[index];
}

template<typename Type>
const Type& SimpleVector<Type>::At(size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("");
    }
    return items_[index];
}

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return (&lhs == &rhs) || (lhs.GetSize() == rhs.GetSize() && std::equal(lhs.begin(), lhs.end(), rhs.begin()));
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs >= lhs;
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    return !(lhs < rhs);
}