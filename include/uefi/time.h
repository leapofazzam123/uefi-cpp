#pragma once

#include <cstdint>

namespace Uefi {
    struct Time {
        uint16_t year{};
        uint8_t month{};
        uint8_t day{};
        uint8_t hour{};
        uint8_t minute{};
        uint8_t second{};
        uint32_t nanosecond{};
        int16_t timezone = 0x7ff;

        uint8_t _pad{};

        uint8_t adjust_daylight{};
        uint8_t in_daylight{};
    };

    struct TimeCapabilities {
        uint32_t resolution;
        uint32_t accuracy;
        bool sets_to_zero;
    };
} // namespace Uefi
