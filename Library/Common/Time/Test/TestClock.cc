// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include <gtest/gtest.h>
#include "Library/Common/Time/Clock.hpp" // Include the header file for your Clock class

constexpr uint64_t OneMillisecond = 1000000;
constexpr uint64_t OneSecond = 1000000000;
constexpr uint64_t TenMicroSecond = 10000;
constexpr uint64_t FiveHundredMicroSecond = 500000;

// Test fixture for the Clock class
class ClockTest : public ::testing::Test {
 public:
    Clock clock; // Create an instance of the Clock class for testing
};

// Test case for the CurrentTimeNanoseconds method
TEST_F(ClockTest, CurrentTimeNanoseconds) {
    uint64_t current_time = clock.CurrentTimeNanoseconds();
    // You can add assertions here to check if current_time is reasonable
    // For example, check if current_time is greater than or equal to zero.
    EXPECT_GE(current_time, 0);
}

// Test case for the SleepForNanoseconds method
TEST_F(ClockTest, SleepForNanoseconds) {
    uint64_t sleep_duration = OneMillisecond; // Sleep for 1 millisecond
    uint64_t start_time = clock.CurrentTimeNanoseconds();
    clock.SleepForNanoseconds(sleep_duration);
    uint64_t end_time = clock.CurrentTimeNanoseconds();
    uint64_t elapsed_time = end_time - start_time;
    // You can add assertions here to check if elapsed_time is close to sleep_duration.
    // For example, you can use EXPECT_NEAR to check if they are within a small tolerance.
    EXPECT_NEAR(elapsed_time, sleep_duration, FiveHundredMicroSecond); // Allow 500 microseconds of tolerance
}

// Test case for the SleepUntilNanoseconds method
TEST_F(ClockTest, SleepUntilNanoseconds) {
    uint64_t current_time = clock.CurrentTimeNanoseconds();
    uint64_t wakeUpTime = current_time + OneSecond; // Wake up after 1 second
    clock.SleepUntilNanoseconds(wakeUpTime);
    uint64_t end_time = clock.CurrentTimeNanoseconds();
    // You can add assertions here to check if end_time is close to wakeUpTime.
    // For example, you can use EXPECT_NEAR to check if they are within a small tolerance.
    EXPECT_NEAR(end_time, wakeUpTime, FiveHundredMicroSecond); // Allow 500 microseconds of tolerance
}

// Test case for the ElapsedTime method
TEST_F(ClockTest, ElapsedTime) {
    clock.Reset();
    clock.SleepForNanoseconds(OneMillisecond); // Sleep for 1 millisecond (1 million nanoseconds)
    uint64_t elapsed_time = clock.ElapsedTime();
    // You can add assertions here to check if elapsed_time is close to the sleep duration.
    // For example, you can use EXPECT_NEAR to check if they are within a small tolerance.
    EXPECT_NEAR(elapsed_time, OneMillisecond, FiveHundredMicroSecond); // Allow 500 microseconds of tolerance
}

// Test case for the Reset method
TEST_F(ClockTest, Reset) {
    clock.Reset();
    uint64_t elapsed_time = clock.ElapsedTime();
    // After resetting, the elapsed time should be close to zero.
    EXPECT_NEAR(elapsed_time, 0, FiveHundredMicroSecond); // Allow 500 microseconds of tolerance
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
