// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <iostream>

#include "Library/ConfigInterface/proto/person.pb.h"
#include "Library/ConfigInterface/ConfigInterface.hpp"

constexpr auto FILE_NAME = "/Vision/Data/Example/ExampleData.txt";
using vision::config::ConfigInterface;

int main() {
    // Create a Config Interface Object
    ConfigInterface<example::Person> config_loader(FILE_NAME);

    auto person = config_loader.GetConfig();
    // Print the values of person
    std::cout << person.id() << ", " << person.name() << ", " << person.email() << std::endl;

  return 0;
}
