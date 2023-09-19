// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#pragma once

#include <chrono>
#include <iostream>
#include <thread>
#include <stdexcept>

namespace vision {
namespace time {
class Clock {
 private:
    uint64_t start_time_;

 public:
    /// @brief Constructor to initialize the start time
    Clock();
    ~Clock() = default;

    /// @brief Get the current time in nanoseconds since epoch
    /// @return The curring time in nanoseconds since epoch
    uint64_t CurrentTimeNanoseconds() const;

    /// @brief Sleep for the specified number of nanoseconds
    /// @param nanoseconds Time to sleep for
    void SleepForNanoseconds(uint64_t nanoseconds);

    /// @brief Sleep until a specific time in nanoseconds since epoch
    /// @param nanoseconds Time to wake up at
    void SleepUntilNanoseconds(uint64_t nanoseconds);

    /// @brief Get the elapsed time in nanoseconds
    /// @return The elapsed time since the Clock was started or reset
    uint64_t ElapsedTime() const;

    /// @brief Reset the start time
    void Reset();

    /// @brief Reset the start time
    void Start();

    /// @brief Stop the start time
    /// @return Returns the elapsed time
    uint64_t Stop() const;

    // Deleted move and copy semantics
    Clock(const Clock& other) = delete;
    Clock(const Clock&& other) = delete;
    Clock operator=(const Clock& other) = delete;
    Clock operator=(const Clock&& other) = delete;
};

}  // namespace time
}  // namespace vision
