// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <array>
#include <functional>

#include "Library/Common/Time/Clock.hpp"
#include "Library/ConfigInterface/ConfigInterface.hpp"

namespace vision {
namespace component {

using vision::time::Clock;

template <typename ConfigType, typename StateType>
class Component {
using OnInitFunctionType=std::function<void(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)>;
using OnUpdateFunctionType=std::function<bool(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)>;
 public:
  /// @brief Constructor that reads commandline argument and loads the config file
  /// @param argc the argument count passed from main function
  /// @param argv the argument vector passed from the main function
  Component(int argc,
            const std::shared_ptr<char**> &argv,
            OnInitFunctionType on_init,
            OnUpdateFunctionType on_update);
  /// @brief The virtual destructor
  virtual ~Component();

  /// @brief The main function that calls OnUpdate on a given freqency
  /// @param frequency The frequency at which the OnUpdate function would be called on a separate thread
  void Run(uint32_t frequency);

  /// @brief Can be called to stop running
  void Stop();

  // Deleted functions
  Component() = delete;

  ConfigType& GetConfig();
  StateType& GetState();

 private:
  std::shared_ptr<ConfigType> config_;
  std::shared_ptr<StateType> state_;

  std::function<void(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)> on_init_function_;
  std::function<bool(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)> on_update_function_;

  Clock clock_;
  std::thread update_thread_;
  std::atomic<bool> exit_requested_ = {false};
  std::mutex state_mutex_;
};

}  // namespace component
}  // namespace vision
