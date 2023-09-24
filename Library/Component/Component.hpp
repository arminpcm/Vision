// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <sys/ipc.h>

#include <array>
#include <functional>

#include "Library/Common/Time/Clock.hpp"
#include "Library/ConfigInterface/ConfigInterface.hpp"
#include "Library/SharedMemory/Publisher.hpp"
#include "Library/SharedMemory/Subscriber.hpp"

namespace vision {
namespace component {

using vision::time::Clock;

template <typename ConfigType, typename StateType>
class Component {
using OnInitFunctionType=std::function<void(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&)>;
using OnUpdateFunctionType=std::function<bool(std::shared_ptr<ConfigType>, std::shared_ptr<StateType>&, std::shared_ptr<std::map<std::string, std::unique_ptr<char>>>&)>;
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

  std::shared_ptr<ConfigType>& GetConfig();
  std::shared_ptr<StateType>& GetState();

  void CreatePublisher(const std::string& topic_name,
                       size_t capacity,
                       size_t message_length);
  void CreateSubscriber(const std::string& topic_name,
                        SubscriberMode mode,
                        size_t message_length,
                        const std::function<void(std::unique_ptr<char>, size_t)> callback);


 private:
  std::shared_ptr<ConfigType> config_;
  std::shared_ptr<StateType> state_;

  std::function<void(const std::shared_ptr<ConfigType>&, std::shared_ptr<StateType>&)> on_init_function_;
  std::function<bool(const std::shared_ptr<ConfigType>&, std::shared_ptr<StateType>&, std::shared_ptr<std::map<std::string, std::unique_ptr<char>>>&)> on_update_function_;

  Clock clock_;
  std::thread update_thread_;
  std::atomic<bool> exit_requested_ = {false};
  std::mutex state_mutex_;

  std::map<std::string, std::shared_ptr<Publisher>> publishers_;    // Use std::map for publishers
  std::map<std::string, std::shared_ptr<Subscriber>> subscribers_;  // Use std::map for subscribers
};

}  // namespace component
}  // namespace vision
