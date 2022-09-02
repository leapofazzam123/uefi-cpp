#pragma once

#include <cstdint>

namespace Uefi {
    /// This structures describes a revision of some structure or software.
    /// For UEFI tables, the revision corresponds to the specification's version.
    struct Revision {
        uint16_t minor;
        uint16_t major;
    };
} // namespace Uefi
