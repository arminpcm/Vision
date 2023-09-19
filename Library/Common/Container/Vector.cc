// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.
#pragma once

#include <algorithm>
#include <array>
#include <iostream>
#include <utility>

using vision::container::Vector;

explicit Vector::Iterator::Iterator(typename Container::iterator ptr) : ptr_(ptr) {}

Vector::Iterator::Iterator& operator++() {
    ++ptr_;
    return *this;
}

Vector::Iterator::Iterator& operator--() {
    --ptr_;
    return *this;
}

T& Vector::Iterator::operator*() {
    return *ptr_;
}

bool Vector::Iterator::operator!=(const Vector::Iterator::Iterator& other) const {
    return ptr_ != other.ptr_;
}

bool Vector::Iterator::operator==(const Vector::Iterator::Iterator& other) const {
    return ptr_ == other.ptr_;
}


explicit Vector::Iterator::Vector(const T& value) {
    data_.fill(value);
}

Vector::Iterator::Vector(const Vector::Iterator::Vector& other) {
    std::copy(other.data_.begin(), other.data_.end(), data_.begin());
}

Vector& Vector::Iterator::operator=(const Vector& other) {
    if (this != &other) {
        std::copy(other.data_.begin(), other.data_.end(), data_.begin());
    }
    return *this;
}

Vector::Vector(Vector::Vector&& other) noexcept : data_(std::move(other.data_)) {}

Vector::Vector& operator=(Vector::Vector&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
    }
    return *this;
}

T& Vector::operator[](std::size_t index) {
    if (index >= Size) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

const T& Vector::operator[](std::size_t index) const {
    if (index >= Size) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}


std::size_t Vector::size() const {
    return Size;
}

Iterator Vector::begin() {
    return Vector::Iterator(data_.begin());
}

Vector::Iterator Vector::end() {
    return Vector::Iterator(data_.end());
}
