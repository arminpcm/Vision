// Copyright 2023 Armin Parchami. All rights reserved.

// This software is provided "as is," without warranty of any kind, express or implied,
// including but not limited to the warranties of merchantability,
// fitness for a particular purpose and noninfringement.
// In no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract,
// tort or otherwise, arising from, out of or in connection with
// the software or the use or other dealings in the software.

#include "Library/Common/Time/Clock.hpp"
#include "Clock.hpp"

using vision::time::Clock;

Clock::Clock() : start_time_(Clock::CurrentTimeNanoseconds()) {
};

uint64_t Clock::CurrentTimeNanoseconds() const {
    auto now = std::chrono::high_resolution_clock::now();
    return std::chrono::time_point_cast<std::chrono::nanoseconds>(now).time_since_epoch().count();
}

void Clock::SleepForNanoseconds(uint64_t nanoseconds) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(nanoseconds));
}

void Clock::SleepUntilNanoseconds(uint64_t nanoseconds) {
    uint64_t currentNanoseconds = Clock::CurrentTimeNanoseconds();
    if (nanoseconds > currentNanoseconds) {
        uint64_t sleepDuration = nanoseconds - currentNanoseconds;
        std::this_thread::sleep_for(std::chrono::nanoseconds(sleepDuration));
    } else {
        throw std::runtime_error("Cannot sleep until a time in the past");
    }
}

uint64_t Clock::ElapsedTime() const {
    uint64_t currentTime = Clock::CurrentTimeNanoseconds();
    return currentTime - start_time_;
}

void Clock::Reset() {
    start_time_ = Clock::CurrentTimeNanoseconds();
}

void Clock::Start() {
    start_time_ = Clock::CurrentTimeNanoseconds();
}

uint64_t Clock::Stop() const {
    uint64_t currentTime = Clock::CurrentTimeNanoseconds();
    return currentTime - start_time_;
}
