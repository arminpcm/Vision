// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <google/protobuf/util/json_util.h>

#include <stdexcept>
#include <iostream>
#include <fstream>
#include <string>

namespace vision {
namespace config {

/// @brief Config interface class that allows users to get serialized representation of config object
/// @tparam T The struct used to define config
template <typename T>
class ConfigInterface {
 public:
    /// @brief deleted default constructor
    ConfigInterface() = delete;

    /// @brief Constructor loading config given a filename
    /// @param file_name The path to the config file
    explicit ConfigInterface(const std::string& file_name) noexcept;

    /// @brief Default destructor
    ~ConfigInterface() = default;

    /// @brief Loads config
    void LoadConfig();

    // Deleted move and copy semantics
    ConfigInterface(const ConfigInterface& other) = delete;
    ConfigInterface(const ConfigInterface&& other) = delete;
    ConfigInterface operator=(const ConfigInterface& other) = delete;
    ConfigInterface operator=(const ConfigInterface&& other) = delete;

    /// @brief Protected method defined to provide access to config_ object for children
    /// @return Representing the config
    T GetConfig() const;

 private:
    T config_object_;
    std::string file_name_;
};

}  // namespace config
}  // namespace vision

#include "Library/ConfigInterface/ConfigInterface.cc"
