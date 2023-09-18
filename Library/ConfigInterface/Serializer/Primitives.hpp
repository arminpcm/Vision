// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <yaml-cpp/yaml.h>

namespace vision {
namespace config {
namespace serializer {

struct UInt8 {
    uint8_t data_;
    UInt8 Serialize(YAML::Node& node) {
        data_ = node.as<uint8_t>();
        return *this;
    }
};

struct UInt16 {
    uint16_t data_;
    UInt16 Serialize(YAML::Node& node) {
        data_ = node.as<uint16_t>();
        return *this;
    }
};

struct UInt32 {
    uint32_t data_;
    UInt32 Serialize(YAML::Node& node) {
        data_ = node.as<uint32_t>();
        return *this;
    }
};

struct UInt64 {
    uint64_t data_;
    UInt64 Serialize(YAML::Node& node) {
        data_ = node.as<uint64_t>();
        return *this;
    }
};

struct Float32 {
    float32 data_;
    Float32 Serialize(YAML::Node& node) {
        data_ = node.as<float32>();
        return *this;
    }
};

struct Float64 {
    float64 data_;
    Float64 Serialize(YAML::Node& node) {
        data_ = node.as<float64>();
        return *this;
    }
};

struct String {
    std::string data_;
    String Serialize(YAML::Node& node) {
        data_ = node.as<std::string>();
        return *this;
    }
};

struct Bool {
    bool data_;
    Bool Serialize(YAML::Node& node) {
        data_ = node.as<bool>();
        return *this;
    }
};


}  // namespace serliazer
}  // namespace config
}  // namespace vision