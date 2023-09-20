// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Library/Common/Container/Vector.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <utility>

namespace vision {
namespace container {

template <typename T, std::size_t Size>
Vector<T, Size>::Iterator::Iterator(typename Container::iterator ptr) : ptr_(ptr) {}

template <typename T, std::size_t Size>
typename Vector<T, Size>::Iterator& Vector<T, Size>::Iterator::operator++() {
    ++ptr_;
    return *this;
}

template <typename T, std::size_t Size>
typename Vector<T, Size>::Iterator& Vector<T, Size>::Iterator::operator--() {
    --ptr_;
    return *this;
}

template <typename T, std::size_t Size>
T& Vector<T, Size>::Iterator::operator*() {
    return *ptr_;
}

template <typename T, std::size_t Size>
bool Vector<T, Size>::Iterator::operator!=(const Vector<T, Size>::Iterator& other) const {
    return ptr_ != other.ptr_;
}

template <typename T, std::size_t Size>
bool Vector<T, Size>::Iterator::operator==(const Vector<T, Size>::Iterator& other) const {
    return ptr_ == other.ptr_;
}

template <typename T, std::size_t Size>
Vector<T, Size>::Vector(const T& value) {
    data_.fill(value);
}

template <typename T, std::size_t Size>
Vector<T, Size>::Vector(const Vector<T, Size>& other) {
    std::copy(other.data_.begin(), other.data_.end(), data_.begin());
}

template <typename T, std::size_t Size>
Vector<T, Size>& Vector<T, Size>::operator=(const Vector<T, Size>& other) {
    if (this != &other) {
        std::copy(other.data_.begin(), other.data_.end(), data_.begin());
    }
    return *this;
}

template <typename T, std::size_t Size>
Vector<T, Size>::Vector(Vector<T, Size>&& other) noexcept : data_(std::move(other.data_)) {
}

template <typename T, std::size_t Size>
Vector<T, Size>& Vector<T, Size>::operator=(Vector<T, Size>&& other) noexcept {
    if (this != &other) {
        data_ = std::move(other.data_);
    }
    return *this;
}

template <typename T, std::size_t Size>
T& Vector<T, Size>::operator[](std::size_t index) {
    if (index >= Size) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T, std::size_t Size>
const T& Vector<T, Size>::operator[](std::size_t index) const {
    if (index >= Size) {
        throw std::out_of_range("Index out of range");
    }
    return data_[index];
}

template <typename T, std::size_t Size>
std::size_t Vector<T, Size>::size() const {
    return Size;
}

template <typename T, std::size_t Size>
typename Vector<T, Size>::Iterator Vector<T, Size>::begin() {
    return Vector<T, Size>::Iterator(data_.begin());
}

template <typename T, std::size_t Size>
typename Vector<T, Size>::Iterator Vector<T, Size>::end() {
    return Vector<T, Size>::Iterator(data_.end());
}

}  // namespace container
}  // namespace vision
