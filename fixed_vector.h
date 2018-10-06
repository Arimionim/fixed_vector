#ifndef MY_VECTOR_VECTOR_H
#define MY_VECTOR_VECTOR_H

#include <iostream>
#include <type_traits>
#include <string>

template<class T, size_t N>
class fixed_vector {
    typename std::aligned_storage<sizeof(T), alignof(T)>::type data[N];
    size_t size_ = 0;

    typedef T* iterator;
    typedef const T* const_iterator;
    typedef std::reverse_iterator<iterator> reverse_iterator;
    typedef std::reverse_iterator<const_iterator > const_reverse_iterator;
public:
    fixed_vector() = default;

    fixed_vector(fixed_vector const &other): size_(other.size_) {
        for (size_t i = 0; i < size_; i++)
            data[i] = (other.data[i]);
    }

    fixed_vector &operator=(fixed_vector const &other) {
        clear();
        size_ = other.size_;
        for (size_t i = 0; i < size_; i++)
            data[i] = (other.data[i]);
        return *this;
    }

    ~fixed_vector() {
        clear();
    }

    void push_back(T const& x) {
        if (size_ == max_size())
            throw std::length_error("cant push_back, vector is overflowed");
        new(&data[size_++])T(x);
    }

    void pop_back() {
        if (empty())
            throw std::length_error("cant pop_back is empty");
        reinterpret_cast<T *>(data + size_ - 1)->~T();
        size_--;
    }

    size_t size() const {
        return size_;
    }

    bool empty() const {
        return size_ == 0;
    }

    size_t capacity() const {
        return N;
    }

    size_t max_size() const {
        return capacity();
    }

    T& back() {
        return *(reinterpret_cast<T*>(data) + size_ - 1);
    }

    T const& back() const {
        return *(reinterpret_cast<T *>(data) + size_ - 1);
    }

    T& front() {
        return *(reinterpret_cast<T *>(data));
    }

    T const & front() const {
        return data[0];
    }

    iterator insert(const_iterator pos, T const& value) {
        if (size_ == max_size())
            throw std::length_error("cant insert, vector is overflowed");
        size_t ind = pos - begin();
        for (size_t i = size_; i > ind; i--)
            data[i] = data[i - 1];
        size_++;
        *(begin() + ind) = value;
        return begin();
    }


    iterator erase(const_iterator it) {
        iterator pos = begin() + (it - begin());
        return erase(pos, pos + 1);
    }


    iterator erase(const_iterator l, const_iterator r) {
        iterator left = begin() + (l - begin());
        iterator right = begin() + (r - begin());
        iterator ans = left;
        for (;right != end(); right++,left++) {
            *left = *right;
        }
        for (size_t i = (size_t)(left - begin()); i < size_; i++) {
            reinterpret_cast<T *>(data + i)->~T();
        }
        size_ = left - begin();
        return ans;
    }

    void clear() {
        for (size_t i = 0; i < size_; i++)
            reinterpret_cast<T *>(data + i)->~T();
        size_ = 0;
    }

    iterator begin() {
        return reinterpret_cast<T*>(data);
    }

    const_iterator begin() const {
        return reinterpret_cast<T const*>(data);
    }

    iterator end() {
        return reinterpret_cast<T*>(data) + size_;
    }

    const_iterator end() const {
        return reinterpret_cast<T const*>(data) + size_;
    }

    reverse_iterator rbegin() {
        return reverse_iterator(end());
    }

    const_reverse_iterator rbegin() const {
        return const_reverse_iterator(end());
    }

    reverse_iterator rend() {
        return reverse_iterator(begin());
    }

    const_reverse_iterator rend() const {
        return const_reverse_iterator(begin());
    }

    T &operator[](size_t n) {
        return reinterpret_cast<T*>(data)[n];
    }

    T const &operator[](size_t n) const {
        return reinterpret_cast<T const*>(data)[n];
    }

    void swap(fixed_vector &other){
        std::swap(data, other.data);
        std::swap(size_, other.size_);
    }
};

template <typename T>
void swap(T a, T b){
    a.swap(b);
}

#endif //MY_VECTOR_VECTOR_H