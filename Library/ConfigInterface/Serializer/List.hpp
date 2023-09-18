// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <yaml-cpp/yaml.h>
#include "Library/Common/Container/Vector.hpp"

namespace vision {
namespace config {
namespace serializer {

template <typename T, std::size_t Size>
struct List {
    Vector<T, Size> data_;
    List Serialize(YAML::Node& node) {
        for (size_t i = 0; i < node.size(); ++i) {
            data_[i] = node[i].as<T>();
        }
        return *this;
    }
};

}  // namespace serliazer
}  // namespace config
}  // namespace vision