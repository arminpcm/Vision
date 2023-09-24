// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Application/Example/ExampleComponentLibrary/ExampleComponentLibrary.hpp"
#include "Library/Component/Component.hpp"
#include "Library/Component/ComponentImpl.hpp"


using vision::component::Component;

constexpr float FREQUENCY = 10.0;
constexpr auto TOPIC_NAME = "example_topic";
constexpr int QUEUE_SIZE = 10;

int main(int argc, char* argv[]) {
    try {
        std::cout << "Starting!\n";
        std::shared_ptr<char**> shared_argv(&argv);
        std::cout << "Creating component!\n";
        Component<example::ExampleConfig, uint8_t> example_component(
            argc,
            shared_argv,
            vision::component::OnInit,
            vision::component::OnUpdate
        );
        auto message_size = sizeof(msg::ExampleMessage);
        std::cout << "Message length: " << message_size << std::endl;
        std::cout << "Adding publisher!\n";
        // Create publishers and subscribers
        example_component.CreatePublisher(TOPIC_NAME, QUEUE_SIZE, message_size);
        std::cout << "Adding subscriber!\n";
        example_component.CreateSubscriber(TOPIC_NAME, SubscriberMode::GET_LAST, message_size, vision::component::ExampleCallback);

        std::cout << "Running!\n";
        example_component.Run(FREQUENCY);
    } catch (...) {
        std::cout << "An exception happened!" << std::endl;
    }
}
