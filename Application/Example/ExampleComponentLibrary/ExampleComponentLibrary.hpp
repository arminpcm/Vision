// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <memory>
#include <iostream>
#include "Library/ConfigInterface/proto/example_config.pb.h"
#include "Library/SharedMemory/messages/ExampleMessage.hpp"

namespace vision{
namespace component {

constexpr int MAX_ITER = 10;

/// @brief The OnInit function that will be called during initialization of the component
/// @param config The input argument that component fills out from the config file
/// @param state The state struct (protobuf or c++ struct) that OnInit and OnUpdate can access and modify
/// @return True if successful, false if an error occured
bool OnInit(std::shared_ptr<example::ExampleConfig> config, std::shared_ptr<uint8_t>& state) {
  std::cout << "OnInit is called!" << std::endl;
  state = std::make_shared<uint8_t>(0);
  std::cout << "State is: " << std::to_string(*state) << std::endl;
  std::cout << "Config is: " << config->DebugString() << std::endl;
  return true;
}

/// @brief The OnUpdate function that will be called during execution loop of the component
/// @param config The input argument that component fills out from the config file
/// @param state The state struct (protobuf or c++ struct) that OnInit and OnUpdate can access and modify
/// @return True if successful, false if an error occured
bool OnUpdate(std::shared_ptr<example::ExampleConfig> config,
              std::shared_ptr<uint8_t>& state,
              std::shared_ptr<std::map<std::string, std::unique_ptr<char>>>& outputs) {
    (*state)++;
    std::cout << "OnUpdate is called for " << std::to_string(*state) << "th time!" << std::endl;

    // Publish a message
    msg::ExampleMessage message;
    message.id = static_cast<uint8_t>(*state);

    // Serialize the message to a byte array
    size_t size = sizeof(msg::ExampleMessage);
    std::unique_ptr<char> buffer(new char[size]);
    std::memcpy(buffer.get(), &message, size);

    // Insert the serialized message into the outputs map
    outputs->insert({"example_topic", std::move(buffer)});
    std::cout << "Publishing this message: id=" << std::to_string(message.id) << std::endl;

    // Decide when to exit
    if (*state < MAX_ITER) {
        return true;
    } else {
        return false;
    }
}

/// @brief The callback for an example message
/// @param message The input message
/// @param length The length of the message
void ExampleCallback(std::unique_ptr<char> message, size_t length) {
    msg::ExampleMessage received_message;

    std::memcpy(&received_message, message.get(), sizeof(msg::ExampleMessage));
    
    std::cout << "Got this message: id=" << std::to_string(received_message.id) << std::endl;
}

}  // namespace component
}  // namespace vision
