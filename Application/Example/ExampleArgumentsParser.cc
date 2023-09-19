// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <gflags/gflags.h>

#include <iostream>

DEFINE_string(topic_name, "", "Name of the topic");  // NOLINT
DEFINE_int32(queue_size, 0, "Size of the queue");    // NOLINT


int main(int argc, char* argv[]) {
    // Initialize gflags
    gflags::ParseCommandLineFlags(&argc, &argv, true);

    // Check if the required flags are provided
    if (FLAGS_topic_name.empty()) {
        std::cerr << "Error: topic_name flag is required." << std::endl;
        return 1;
    }

    if (FLAGS_queue_size <= 0) {
        std::cerr << "Error: queue_size must be a positive integer." << std::endl;
        return 1;
    }

    // Access the parsed values
    std::string topic_name = FLAGS_topic_name;
    int queue_size = FLAGS_queue_size;

    // Use the parsed values in your application
    std::cout << "Topic Name: " << topic_name << std::endl;
    std::cout << "Queue Size: " << queue_size << std::endl;

    return 0;
}
