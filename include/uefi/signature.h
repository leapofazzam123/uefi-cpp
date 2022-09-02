#pragma once

#include <cstdint>

namespace Uefi {
    /// 64-bit value to identify tables.
    struct Signature {
        constexpr Signature(uint64_t signature)
            : value{signature} {
        }

        constexpr operator uint64_t() const {
            return value;
        }

        uint64_t value;
    };

    constexpr bool operator==(Signature lhs, Signature rhs) {
        return lhs.value == rhs.value;
    }
} // namespace Uefi
