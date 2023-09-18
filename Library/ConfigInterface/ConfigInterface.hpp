// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <stdexcept>
#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>

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
    /// @param file_name The path to the YAML file
    explicit ConfigInterface(const std::string& file_name) noexcept : file_name_(file_name) {
        LoadConfig();
    };

    /// @brief Default destructor defined as virtual to allow inheritence
    virtual ~ConfigInterface() = default;

    /// @brief Loads config
    virtual void LoadConfig() {
        try {
            config_ = YAML::LoadFile(file_name_);
        } catch (const YAML::Exception& e) {
            std::cerr << "Error parsing YAML: " << e.what() << std::endl;
        }
    };

    /// @brief Serializes the config
    /// @return Returns serialized representation of the config
    /// @throw Throws std::runtime if the config_ object is empty
    T GetConfigObject() {
        if (config_.IsNull()) {
            throw std::runtime_error("Cannot serialize an empty config");
        }
        return config_object_.Serialize(config_);
    }

    // Deleted move and copy semantics 
    ConfigInterface(ConfigInterface& other) = delete;
    ConfigInterface(ConfigInterface&& other) = delete;
    ConfigInterface operator=(ConfigInterface& other) = delete;
    ConfigInterface operator=(ConfigInterface&& other) = delete;

protected:
    /// @brief Protected method defined to provide access to config_ object for children
    /// @return YAML::Node representing the config
    YAML::Node GetConfig() const { return config_; };

private:
    YAML::Node config_;
    T config_object_;
    std::string file_name_;
};
}  // namespace config
}  // namespace vision