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
constexpr auto TOPIC_NAME = "/example_topic_1";
constexpr int NAMESPACE = 0;
constexpr int QUEUE_SIZE = 10;

int main(int argc, char* argv[]) {
    try {
        std::shared_ptr<char**> shared_argv(&argv);
        Component<example::ExampleConfig, uint8_t> example_component(argc,
            shared_argv,
            vision::component::OnInit,
            vision::component::OnUpdate);

        // Create publishers and subscribers
        example_component.CreatePublisher(TOPIC_NAME, NAMESPACE, QUEUE_SIZE, sizeof(example::ExampleMessage));
        example_component.CreateSubscriber(TOPIC_NAME, NAMESPACE, SubscriberMode::GET_FIRST, sizeof(example::ExampleMessage), vision::component::ExampleCallback);

        example_component.Run(FREQUENCY);
    } catch (...) {
        std::cout << "An exception happened!" << std::endl;
    }
}
