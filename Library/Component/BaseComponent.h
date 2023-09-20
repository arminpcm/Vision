// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <gflags/gflags.h>

#include <array>

#include "Library/ConfigInterface/ConfigInterface.hpp"
#include "Library/Common/Time/Clock.hpp"
#include "Library/ConfigInterface/ConfigInterface.hpp"

namespace vision {
namespace component {

using vision::time::Clock;

DEFINE_string(config_path, "", "Path to the config file");  // NOLIT

template <typename ConfigType, typename StateType>
class BaseComponent {
 public:
   /// @brief Constructor that reads commandline argument and loads the config file
   /// @param argc the argument count passed from main function
   /// @param argv the argument vector passed from the main function
   BaseComponent(int argc, const std::shared_ptr<char**> &argv);

   /// @brief The virtual destructor
   virtual ~BaseComponent();

   /// @brief Users can override this function to perform initialization tasks
   virtual void OnInit() = 0;

   /// @brief Users can override this function to update the component
   virtual void OnUpdate() = 0;

   /// @brief The main function that calls OnUpdate on a given freqency
   /// @param frequency The frequency at which the OnUpdate function would be called on a separate thread
   void Run(uint32_t frequency);

   /// @brief Can be called to stop running
   void Stop();

   // Deleted functions
   BaseComponent() = delete;

 private:
   ConfigType config_;
   StateType state_;
   Clock clock_;
   std::thread update_thread_;
   std::atomic<bool> exit_requested_ = {false};

 protected:
   ConfigType& GetConfig();
   StateType& GetState();
};

}  // namespace component
}  // namespace vision
