#pragma once

#include <chrono>
#include <cstdint>

inline uint64_t getCurrentTimestamp() {
    using namespace std::chrono;
    return duration_cast<nanoseconds>(steady_clock::now().time_since_epoch()).count();
}