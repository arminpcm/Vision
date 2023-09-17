// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <iostream>
#include <string>
#include <yaml-cpp/yaml.h>


class ConfigInterface {
public:
    ConfigInterface() = delete;
    explicit ConfigInterface(const std::string& file_name);
    virtual ~ConfigInterface();

    virtual bool LoadConfig();


    ConfigInterface(ConfigInterface& other) = delete;
    ConfigInterface(ConfigInterface&& other) = delete;
    ConfigInterface operator=(ConfigInterface& other) = delete;
    ConfigInterface operator=(ConfigInterface&& other) = delete;
protected:
    YAML::Node getConfig() const;
private:
    YAML::Node config_;
    std::string file_name_;
};