#include "Library/Common/Example/ExampleLibrary.hpp"
#include <iostream>

int main() {
    constexpr int var_a = 5;
    constexpr int var_b = 6;
    std::cout << add(var_a, var_b) << std::endl;
}