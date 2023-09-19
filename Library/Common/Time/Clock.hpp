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


class Clock {
 private:
    uint64_t start_time_;

 public:
    /// @brief Constructor to initialize the start time
    Clock() : start_time_(CurrentTimeNanoseconds()) {}
    ~Clock() = default;

    /// @brief Get the current time in nanoseconds since epoch
    /// @return The curring time in nanoseconds since epoch
    uint64_t CurrentTimeNanoseconds() const {
        auto now = std::chrono::high_resolution_clock::now();
        return std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
    }

    /// @brief Sleep for the specified number of nanoseconds
    /// @param nanoseconds Time to sleep for
    void SleepForNanoseconds(uint64_t nanoseconds) {
        std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
    }

    /// @brief Sleep until a specific time in nanoseconds since epoch
    /// @param nanoseconds Time to wake up at
    void SleepUntilNanoseconds(uint64_t nanoseconds) {
        uint64_t currentNanoseconds = CurrentTimeNanoseconds();
        if (nanoseconds > currentNanoseconds) {
            uint64_t sleepDuration = nanoseconds - currentNanoseconds;
            std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
        } else {
            throw std::runtime_error("Cannot sleep until a time in the past");
        }
    }

    /// @brief Get the elapsed time in nanoseconds
    /// @return The elapsed time since the Clock was started or reset
    uint64_t ElapsedTime() const {
        uint64_t currentTime = CurrentTimeNanoseconds();
        return currentTime - start_time_;
    }

    /// @brief Reset the start time
    void Reset() {
        start_time_ = CurrentTimeNanoseconds();
    }

    /// @brief Reset the start time
    void Start() {
        start_time_ = CurrentTimeNanoseconds();
    }

    /// @brief Stop the start time
    /// @return Returns the elapsed time
    uint64_t Stop() const {
        uint64_t currentTime = CurrentTimeNanoseconds();
        return currentTime - start_time_;
    }

    // Deleted move and copy semantics
    Clock(const Clock& other) = delete;
    Clock(const Clock&& other) = delete;
    Clock operator=(const Clock& other) = delete;
    Clock operator=(const Clock&& other) = delete;
};
