// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include "Library/Component/Component.hpp"

#include <gflags/gflags.h>

#include <atomic>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <thread>
#include <utility>

namespace vision {
namespace component {

using vision::config::ConfigInterface;

DEFINE_string(config_path, "", "Path to the config file");  // NOLINT

template <typename ConfigType, typename StateType>  // NOLINT
Component<ConfigType, StateType>::Component(  // NOLINT
    int argc,
    const std::shared_ptr<char**> &argv,
    OnInitFunctionType on_init,
    OnUpdateFunctionType on_update) :
        on_init_function_(std::move(on_init)),
        on_update_function_(std::move(on_update)) {
    // Initialize gflags
    gflags::ParseCommandLineFlags(&argc, &*argv.get(), true);
    // Check if the required flags are provided
    if (FLAGS_config_path.empty()) {
        std::cerr << "Error: config_path flag is required." << std::endl;
        throw std::runtime_error("Providing config_path flag is mandatory");
    }

    // Load configuration and create publishers and subscribers
    ConfigInterface<ConfigType> config_loader{std::string(FLAGS_config_path)};
    config_ = std::make_shared<ConfigType>(config_loader.GetConfig());

    is_initialized_ = false;
}

template <typename ConfigType, typename StateType>
key_t Component<ConfigType, StateType>::GenerateKeyFromTopic(const std::string& topic_name, int name_space) {
    // Generate the key using ftok with the topic_name and a namespace
    key_t key = ftok(topic_name.c_str(), name_space);

    if (key == -1) {
        throw std::runtime_error("Could not generate a unique key for the given topic name and namespace");
    }

    return key;
}

template <typename ConfigType, typename StateType>
void Component<ConfigType, StateType>::Run(uint32_t frequency) {
    if (is_initialized_ == false) {
        throw std::runtime_error("The component is not initialized");
    }
    // Call OnInit before starting the update loop
    on_init_function_(config_, state_);

    // Calculate the time interval between updates
    const uint64_t update_interval_ns = 1000000000 / frequency; // Frequency in Hz to nanoseconds

    // Start the update thread
    update_thread_ = std::thread([this, update_interval_ns]() {
        while (!exit_requested_) {
            clock_.Reset();
            
            {
                std::lock_guard<std::mutex> lock(state_mutex_); // Lock the mutex

                // Call the user-defined update function
                auto return_flag = on_update_function_(config_, state_);

                if (return_flag == false) {
                    exit_requested_ = true;
                }
            }

            // Calculate how much time to sleep to achieve the desired update frequency
            auto elapsed_time = clock_.ElapsedTime();
            if (elapsed_time < update_interval_ns) {
                clock_.SleepForNanoseconds(update_interval_ns - elapsed_time);
            }
        }
    });
}

template <typename ConfigType, typename StateType>
Component<ConfigType, StateType>::~Component() {
    if (update_thread_.joinable()) {
        update_thread_.join();
    }
    config_.reset();
    state_.reset();
}

template <typename ConfigType, typename StateType>
void Component<ConfigType, StateType>::Stop() {
    exit_requested_ = true;
}

template <typename ConfigType, typename StateType>
std::shared_ptr<ConfigType>& Component<ConfigType, StateType>::GetConfig() {
    return config_;
}

template <typename ConfigType, typename StateType>
std::shared_ptr<StateType>& Component<ConfigType, StateType>::GetState() {
    return state_;
}

template <typename ConfigType, typename StateType>
void Component<ConfigType, StateType>::CreatePublisher(const std::string& topic_name, int name_space, size_t capacity, size_t message_length) {
    // Generate the key using ftok with the topic_name and name_space
    key_t key = GenerateKeyFromTopic(topic_name, name_space);
    
    // Create and add the publisher to the vector
    publishers_.emplace_back(std::make_shared<Publisher>(key, capacity, message_length));
}

template <typename ConfigType, typename StateType>
void Component<ConfigType, StateType>::CreateSubscriber(const std::string& topic_name, int name_space, SubscriberMode mode, size_t message_length,
    const std::function<void(std::unique_ptr<char>, size_t)>& callback) {
    // Generate the key using ftok with the topic_name and name_space
    key_t key = GenerateKeyFromTopic(topic_name, name_space);
    
    // Create and add the subscriber to the vector
    subscribers_.emplace_back(std::make_shared<Subscriber>(key, mode, message_length, callback));
}

}  // namespace component
}  // namespace vision
