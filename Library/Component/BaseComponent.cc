// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Library/Component/BaseComponent.h"

#include <gflags/gflags.h>

#include <atomic>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <utility>

namespace vision {
namespace component {

using vision::config::ConfigInterface;

template <typename ConfigType, typename StateType>
BaseComponent<ConfigType, StateType>::BaseComponent(int argc, const std::shared_ptr<char**>& argv) {
    // Initialize gflags
    gflags::ParseCommandLineFlags(&argc, &*argv.get(), true);
    // Check if the required flags are provided
    if (FLAGS_config_path.empty()) {
        std::cerr << "Error: config_path flag is required." << std::endl;
        throw std::runtime_error("Providing config_path flag is mandatory");
    }

    auto config_interface = ConfigInterface<ConfigType>(std::string(FLAGS_config_path));
}

template <typename ConfigType, typename StateType>
void BaseComponent<ConfigType, StateType>::Run(uint32_t frequency) {
    // Call OnInit before starting the update loop
    OnInit();

    // Calculate the time interval between updates
    const uint64_t update_interval_ns = 1000000000 / frequency; // Frequency in Hz to nanoseconds

    // Start the update thread
    update_thread_ = std::thread([this, update_interval_ns]() {
        while (!exit_requested_) {
            clock_.Reset();
            
            // Call the user-defined update function
            OnUpdate();

            // Calculate how much time to sleep to achieve the desired update frequency
            auto elapsed_time = clock_.ElapsedTime();
            if (elapsed_time < update_interval_ns) {
                clock_.SleepForNanoseconds(update_interval_ns - elapsed_time);
            }
        }
    });
}

template <typename ConfigType, typename StateType>
BaseComponent<ConfigType, StateType>::~BaseComponent() {
    if (update_thread_.joinable()) {
        update_thread_.join();
    }
}

template <typename ConfigType, typename StateType>
void BaseComponent<ConfigType, StateType>::Stop() {
    exit_requested_ = true;
}

template <typename ConfigType, typename StateType>
ConfigType& BaseComponent<ConfigType, StateType>::GetConfig() {
    return config_;
}

template <typename ConfigType, typename StateType>
StateType& BaseComponent<ConfigType, StateType>::GetState() {
    return state_;
}

}  // namespace component
}  // namespace vision
