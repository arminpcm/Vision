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
#include "Library/ConfigInterface/proto/example_config.pb.h"
#include "Library/SharedMemory/messages/ExampleMessage.hpp"

constexpr auto MAX_UPDATE = 10;
constexpr auto FREQUENCY = 10;
constexpr auto SEC_TO_MS = 1000;
constexpr auto SLEEP_TIME = (MAX_UPDATE/FREQUENCY) * SEC_TO_MS;
constexpr auto TOPIC_NAME = "example_topic";
constexpr int QUEUE_SIZE = 10;

namespace vision{
namespace component {

bool OnInit(const std::shared_ptr<example::ExampleConfig>& config, std::shared_ptr<uint8_t>& state) {
  std::cout << "OnInit is called!" << std::endl;
  state = std::make_shared<uint8_t>(0);
  std::cout << "State is: " << std::to_string(*state) << std::endl;
  std::cout << "Config is: " << config->DebugString() << std::endl;
  return true;
}

bool OnUpdate(const std::shared_ptr<example::ExampleConfig>& config,
              std::shared_ptr<uint8_t>& state,
              std::shared_ptr<std::map<std::string, std::unique_ptr<char>>>& outputs) {
    (*state)++;
    std::cout << "OnUpdate is called for " << std::to_string(*state) << "th time!" << std::endl;

    // Publish a message
    msg::ExampleMessage message{static_cast<uint8_t>(*state)};

    // Serialize the message to a byte array
    size_t size = sizeof(msg::ExampleMessage);
    std::unique_ptr<char> buffer(new char[size]);
    std::memcpy(buffer.get(), &message, size);

    // Insert the serialized message into the outputs map
    outputs->insert({"example_topic", std::move(buffer)});
    std::cout << "Publishing this message: id=" << std::to_string(message.id) << std::endl;

    // Decide when to exit
    if (*state < MAX_UPDATE) {
        return true;
    } else {
        return false;
    }
}

void ExampleCallback(std::unique_ptr<char> message, size_t length) {
    msg::ExampleMessage received_message{};

    std::memcpy(&received_message, message.get(), sizeof(msg::ExampleMessage));
    
    std::cout << "Got this message: id=" << std::to_string(received_message.id) << std::endl;
}

}  // namespace component
}  // namespace vision


// Define a test fixture for the Component class
class ComponentTest : public testing::Test {  // NOLINT
 public:
    int argc_ = 2;
    char* argv_[2] = { (char*)"test_executable", (char*)"--config_path=/Vision/Data/Example/ExampleConfig.txt" };  // NOLINT
};

// Define the tests using the test fixture
TEST_F(ComponentTest, RunAndStop) {  // NOLINT
    // Create a shared pointer to char**
    std::shared_ptr<char**> argv_shared = std::make_shared<char**>(argv_);

    vision::component::Component<example::ExampleConfig, uint8_t> component(
        this->argc_,
        argv_shared, 
        vision::component::OnInit,
        vision::component::OnUpdate
    );
    // Initialize the Subscribers
    auto message_size = sizeof(msg::ExampleMessage);
    std::cout << "Message length: " << message_size << std::endl;
    std::cout << "Adding publisher!\n";
    // Create publishers and subscribers
    component.CreatePublisher(TOPIC_NAME, QUEUE_SIZE, message_size);
    std::cout << "Adding subscriber!\n";
    component.CreateSubscriber(TOPIC_NAME, SubscriberMode::GET_LAST, message_size, vision::component::ExampleCallback);

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
