// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include "Library/Component/Component.h"

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

DEFINE_string(config_path, "", "Path to the config file");  // NOLIT

template <typename ConfigType, typename StateType>
Component<ConfigType, StateType>::Component(int argc,
            const std::shared_ptr<char**> &argv,
            std::function<void(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)> on_init,
            std::function<bool(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)> on_update) :
            on_init_function_(std::move(on_init)), on_update_function_(std::move(on_update)) {
    // Initialize gflags
    gflags::ParseCommandLineFlags(&argc, &*argv.get(), true);
    // Check if the required flags are provided
    if (FLAGS_config_path.empty()) {
        std::cerr << "Error: config_path flag is required." << std::endl;
        throw std::runtime_error("Providing config_path flag is mandatory");
    }

    ConfigInterface<ConfigType> config_loader{std::string(FLAGS_config_path)};

    config_ = std::make_shared<ConfigType>(config_loader.GetConfig());
}

template <typename ConfigType, typename StateType>
void Component<ConfigType, StateType>::Run(uint32_t frequency) {
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
ConfigType& Component<ConfigType, StateType>::GetConfig() {
    return config_;
}

template <typename ConfigType, typename StateType>
StateType& Component<ConfigType, StateType>::GetState() {
    return state_;
}

}  // namespace component
}  // namespace vision
