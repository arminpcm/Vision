// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Library/ConfigInterface/ConfigInterface.hpp"
#include "Library/ConfigInterface/proto/person.pb.h"

#include <google/protobuf/util/json_util.h>
#include <iostream>
#include <fstream>
#include <string>

namespace vision {
namespace config {

template <typename T>
ConfigInterface<T>::ConfigInterface(const std::string& file_name) noexcept : file_name_(file_name) {
    LoadConfig();
}

template <typename T>
void ConfigInterface<T>::LoadConfig() {
    // Open the file
    std::ifstream file_stream(file_name_, std::ios::in);

    // Check if the file is open
    if (!file_stream.is_open()) {
        std::cerr << "Error: Could not open the file." << std::endl;
        return;
    }

    // Read the file into a string
    std::string file_contents((std::istreambuf_iterator<char>(file_stream)),
                            std::istreambuf_iterator<char>());

    // Close the file
    file_stream.close();

    // Parse the json_string into protobuf struct
    google::protobuf::util::JsonParseOptions options;
    JsonStringToMessage(file_contents, &config_object_, options);
}

// Define the GetConfig function
template <typename T>
T ConfigInterface<T>::GetConfig() const {
    return config_object_;
}

}  // namespace config
}  // namespace vision

template class vision::config::ConfigInterface<example::Person>;