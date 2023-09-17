// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Library/Common/Example/ExampleLibrary.hpp"
#include <iostream>

/// @brief The main function for the application
int main() {
    constexpr int var_a = 5;
    constexpr int var_b = 6;
    std::cout << add(var_a, var_b) << std::endl;
}