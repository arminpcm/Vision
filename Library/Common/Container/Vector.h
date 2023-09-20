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

namespace vision {
namespace container {

/// @brief Statically sized vector based on std::array
/// @tparam T Type of the data stored in the vector
/// @tparam Size Size of the vector
template <typename T, std::size_t Size>
class Vector {
 public:
    /// @brief Defining container as a sized array
    using Container = std::array<T, Size>;

    /// @brief Iterator class for Vector to help iterate over items
    class Iterator {
     public:
        /// @brief Explicit single parameter constructor
        /// @param ptr Iterator to initialize with
        explicit Iterator(typename Container::iterator ptr);

        /// @brief Next operator
        /// @return Returns the updated iterator
        Iterator& operator++();

        /// @brief Previous operator
        /// @return Returns the updated iterator
        Iterator& operator--();

        /// @brief Dereferences the iterator
        /// @return Returns the item iterator is pointing to
        T& operator*();

        /// @brief Compares to another iterator
        /// @param other The other iterator object
        /// @return true if both iterators are NOT pointing to the same location
        bool operator!=(const Iterator& other) const;

        /// @brief Compares to another iterator
        /// @param other The other iterator object
        /// @return true if both iterators are pointing to the same location
        bool operator==(const Iterator& other) const;

     private:
        /// @brief Underlying pointer
        typename Container::iterator ptr_;
    };

    /// @brief Default constructor
    Vector() = default;
    /// @brief Default destructor
    ~Vector() = default;

    /// @brief Set all elements to value
    /// @param value The value tha all elements will be filled with
    explicit Vector(const T& value);

    /// @brief Copy constructor
    /// @param other The vector that would be used to fill out this vector
    Vector(const Vector& other);

    /// @brief Copy assignment operator
    /// @param other The other vector
    /// @return Reference to this vector
    Vector& operator=(const Vector& other);

    /// @brief Move constructor
    /// @param other The other vector
    Vector(Vector&& other) noexcept;

    /// @brief Move assignment operator
    /// @param other The other vector
    /// @return Reference to this vector
    Vector& operator=(Vector&& other) noexcept;

    /// @brief Get the value of an element
    /// @param index The index of the element starting from zero
    /// @return The value at index
    /// @throw Throws exception for out of range
    T& operator[](std::size_t index);

    /// @brief Get the value of an element
    /// @param index The index of the element starting from zero
    /// @return The value at index
    /// @throw Throws exception for out of range
    const T& operator[](std::size_t index) const;

    /// @brief Returns the size of the vector
    std::size_t size() const;

    /// @brief Returns an iterator referencing to the first element
    Iterator begin();

    /// @brief Returns an iterator referencing to the last element
    Iterator end();

 private:
    /// @brief The underlying std::array holding elements of the vector
    Container data_;
};
}  // namespace container
}  // namespace vision
