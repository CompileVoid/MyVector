#pragma once

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <utility>

template <typename T>
class MyVector {
public:
    MyVector() = default;

    explicit MyVector(std::size_t size)
        : size_(size), capacity_(size * 2) {
        if (capacity_ > 0) {
            arr_ = new T[capacity_]();
        }
    }

    MyVector(std::size_t size, const T& value)
        : size_(size), capacity_(size * 2) {
        if (capacity_ > 0) {
            arr_ = new T[capacity_];
            for (std::size_t i = 0; i < size_; i++) {
                arr_[i] = value;
            }
        }
    }

    MyVector(std::initializer_list<T> ls) : size_(ls.size()), capacity_(size_ * 2)
    {
        if (capacity_ > 0)
        {
            arr_ = new T[capacity_];
            std::copy(ls.begin(), ls.end(), arr_);
        }
    }

    MyVector(const MyVector& other)
        : size_(other.size_), capacity_(other.capacity_) {
        if (capacity_ > 0) {
            arr_ = new T[capacity_];
            std::copy(other.arr_, other.arr_ + size_, arr_);
        }
    }

    MyVector& operator=(const MyVector& other) {
        if (this == &other) {
            return *this;
        }
        T* new_arr = nullptr;
        if (other.capacity_ > 0) {
            new_arr = new T[other.capacity_];
            std::copy(other.arr_, other.arr_ + other.size_, new_arr);
        }
        delete[] arr_;
        arr_ = new_arr;
        capacity_ = other.capacity_;
        size_ = other.size_;
        return *this;
    }

    MyVector(MyVector&& other) noexcept
        : arr_(other.arr_), size_(other.size_), capacity_(other.capacity_) {
        other.arr_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
    }

    MyVector& operator=(MyVector&& other) noexcept {
        if (this == &other) {
            return *this;
        }
        delete[] arr_;
        arr_ = other.arr_;
        size_ = other.size_;
        capacity_ = other.capacity_;
        other.arr_ = nullptr;
        other.capacity_ = 0;
        other.size_ = 0;
        return *this;
    }

    ~MyVector() {
        delete[] arr_;
    }

    T& operator[](std::size_t index) noexcept {
        return arr_[index];
    }

    const T& operator[](std::size_t index) const noexcept {
        return arr_[index];
    }

    T& at(std::size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return arr_[index];
    }

    [[nodiscard]] const T& at(std::size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("Index out of bounds");
        }
        return arr_[index];
    }

    T* begin() noexcept { return arr_; }
    T* end() noexcept { return arr_ + size_; }
    [[nodiscard]] const T* begin() const noexcept { return arr_; }
    [[nodiscard]] const T* end() const noexcept { return arr_ + size_; }

    [[nodiscard]] bool isEmpty() const noexcept { return size_ == 0; }
    [[nodiscard]] std::size_t size() const noexcept { return size_; }
    [[nodiscard]] std::size_t capacity() const noexcept { return capacity_; }

    void reserve(std::size_t new_capacity) {
        if (new_capacity <= capacity_) {
            return;
        }
        T* temp = new T[new_capacity];
        for (std::size_t i = 0; i < size_; i++) {
            temp[i] = std::move(arr_[i]);
        }
        delete[] arr_;
        arr_ = temp;
        capacity_ = new_capacity;
    }

    void clear() noexcept {
        size_ = 0;
        capacity_ = 0;
        delete[] arr_;
        arr_ = nullptr;
    }

    void pushBack(const T& value) {
        if (size_ == capacity_) {
            reallocate();
        }
        arr_[size_] = value;
        size_++;
    }

    void pushBack(T&& value) {
        if (size_ == capacity_) {
            reallocate();
        }
        arr_[size_] = std::move(value);
        size_++;
    }

    void popBack() {
        if (size_ == 0) {
            throw std::out_of_range("Vector is empty");
        }
        size_--;
    }

    void resize(std::size_t new_size) {
        if (new_size < size_) {
            for (std::size_t i = new_size; i < size_; i++) {
                arr_[i] = T();
            }
            size_ = new_size;
        } else if (new_size > size_) {
            if (new_size > capacity_) {
                reserve(new_size);
            }
            for (std::size_t i = size_; i < new_size; i++) {
                arr_[i] = T();
            }
            size_ = new_size;
        }
    }

    void resize(std::size_t new_size, const T& value) {
        if (new_size < size_) {
            for (std::size_t i = new_size; i < size_; i++) {
                arr_[i] = T();
            }
            size_ = new_size;
        } else if (new_size > size_) {
            if (new_size > capacity_) {
                reserve(new_size);
            }
            for (std::size_t i = size_; i < new_size; i++) {
                arr_[i] = value;
            }
            size_ = new_size;
        }
    }

private:
    T* arr_{nullptr};
    std::size_t size_{0};
    std::size_t capacity_{0};

    void reallocate() {
        std::size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
        reserve(new_capacity);
    }
};

template <typename T>
std::ostream& operator<<(std::ostream& os, const MyVector<T>& vec) {
    for (const auto& item : vec) {
        os << item << ' ';
    }
    os << std::endl;
    return os;
}
