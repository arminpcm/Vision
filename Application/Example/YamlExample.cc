// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <iostream>
#include <yaml-cpp/yaml.h>

int main() {
    try {
        // Load a YAML file
        YAML::Node config = YAML::LoadFile("Data/Example/ExampleYamlConfig.yaml");

        // Access values in the YAML file
        std::string name = config["name"].as<std::string>();
        int age = config["age"].as<int>();

        // Print the values
        std::cout << "Name: " << name << std::endl;
        std::cout << "Age: " << age << std::endl;
    } catch (const YAML::Exception& e) {
        std::cerr << "Error parsing YAML: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
