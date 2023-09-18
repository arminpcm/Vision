// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <gtest/gtest.h>
#include "Library/Common/Container/Vector.hpp"

constexpr int VECTOR_SIZE = 5;
using vision::container::Vector;


class VectorTest : public ::testing::TestWithParam<std::pair<int, int>> {
 public:
    VectorTest(): vector(Vector<int, VECTOR_SIZE>()) {}
    Vector<int, VECTOR_SIZE> vector;

 protected:
    void SetUp() override {
        int initialValue = GetParam().first;
        vector = Vector<int, VECTOR_SIZE>(initialValue);
    }
};


TEST_P(VectorTest, ElementAccess) {
    int expectedValue = GetParam().second;

    // Test element access
    for (std::size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(vector[i], expectedValue);
    }
}

TEST_P(VectorTest, CopyConstructor) {
    Vector<int, VECTOR_SIZE> copyVector(vector);

    // Test that the copy constructor creates an identical copy
    for (std::size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(copyVector[i], vector[i]);
    }
}

TEST_P(VectorTest, CopyAssignmentOperator) {
    Vector<int, VECTOR_SIZE> copyVector(0);
    copyVector = vector;

    // Test that the copy assignment operator creates an identical copy
    for (std::size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(copyVector[i], vector[i]);
    }
}

TEST_P(VectorTest, MoveConstructor) {
    Vector<int, VECTOR_SIZE> original(vector);
    Vector<int, VECTOR_SIZE> moveVector(std::move(original));

    // Test that the move constructor transfers ownership and leaves the original in a valid state
    for (std::size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(moveVector[i], vector[i]);
    }
}

TEST_P(VectorTest, MoveAssignmentOperator) {
    Vector<int, VECTOR_SIZE> original(vector);
    Vector<int, VECTOR_SIZE> moveVector(0);
    moveVector = std::move(original);

    // Test that the move assignment operator transfers ownership and leaves the original in a valid state
    for (std::size_t i = 0; i < vector.size(); ++i) {
        EXPECT_EQ(moveVector[i], vector[i]);
    }
}

TEST_P(VectorTest, Iterator) {
    int initialValue = GetParam().first;

    // Test iterator functionality
    for (auto it = vector.begin(); it != vector.end(); ++it) {
        EXPECT_EQ(*it, initialValue);
    }
}


INSTANTIATE_TEST_SUITE_P(InitialValues, VectorTest, ::testing::Values(
    std::make_pair(0, 0),   // Initial value is 0, expected value is 0
    std::make_pair(1, 1),   // Initial value is 1, expected value is 1
    std::make_pair(-1, -1)  // Initial value is -1, expected value is -1
));


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
