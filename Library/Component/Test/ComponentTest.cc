// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.


#include <gtest/gtest.h>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <memory>

#include "Library/Component/Component.hpp"
#include "Library/Component/ComponentImpl.hpp"
#include "Library/ConfigInterface/proto/person.pb.h"

constexpr auto MAX_UPDATE = 10;
constexpr auto FREQUENCY = 10;
constexpr auto SEC_TO_MS = 1000;
constexpr auto SLEEP_TIME = (MAX_UPDATE/FREQUENCY) * SEC_TO_MS;

namespace vision{
namespace component {

// Define a mock for the OnInit and OnUpdate functions
bool OnInit(std::shared_ptr<example::Person> config, std::shared_ptr<uint8_t>& state) {  // NOLINT
  state = std::make_shared<uint8_t>(0);
  std::cout << config->id() << ", " << config->name() << ", " << config->email() << std::endl;
  return true;
}

bool OnUpdate(std::shared_ptr<example::Person> config, std::shared_ptr<uint8_t>& state) {  // NOLINT
  (*state)++;
  config->set_id(*state);
  if (*state < MAX_UPDATE) {
    return true;
  } else {
    return false;
  }
}

}  // namespace component
}  // namespace vision


// Define a test fixture for the Component class
class ComponentTest : public testing::Test {  // NOLINT
 public:
    int argc_ = 2;
    char* argv_[2] = { (char*)"test_executable", (char*)"--config_path=/Vision/Data/Example/ExampleData.txt" };  // NOLINT
};

// Define the tests using the test fixture
TEST_F(ComponentTest, RunAndStop) {  // NOLINT
    // Create a shared pointer to char**
    std::shared_ptr<char**> argv_shared = std::make_shared<char**>(argv_);

    vision::component::Component<example::Person, uint8_t> component(
                this->argc_,
                argv_shared, 
                vision::component::OnInit,
                vision::component::OnUpdate
            );

    // Start the component and let it run for a while (for demonstration purposes)
    component.Run(FREQUENCY);

    // Sleep for a while to simulate component running
    std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME));

    // Stop the component
    component.Stop();
    std::cout << "\n" << std::to_string(*component.GetState()) << "\n";
    EXPECT_TRUE(std::to_string(*component.GetState()) == "10");
}

// Define the main function to run the tests
int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
