#pragma once

#include <cstdint>

namespace Uefi {
    /// Sets the high-order bit of a number.
    constexpr uint64_t makeErrorCode(uint64_t value) {
        return value | (1ULL << 63);
    }

    /// UEFI interfaces return a Status code for success, errors, and warnings, respectively.
    /// Taken from "Appendix D Status Codes" of the UEFI specification.
    // TODO: add more
    enum class Status : uint64_t {
        /// The operation completed successfully.
        Success = 0,
        /// The image failed to load.
        LoadError = makeErrorCode(1),
        /// A parameter was incorrect.
        InvalidParameter = makeErrorCode(2),
        /// The operation is not supported.
        Unsupported = makeErrorCode(3),
        /// The buffer was not the proper size for the request.
        BadBufferSize = makeErrorCode(4),
        /// The buffer is not large enough to hold the requested data.
        /// The required buffer size is returned in the appropriate parameter when this error occurs.
        BufferTooSmall = makeErrorCode(5),
        /// The item was not found.
        NotFound = makeErrorCode(14),
        /// The function was not performed due to a security violation.
        SecurityViolation = makeErrorCode(26),
    };

    /// Error codes have the high-order bit set.
    constexpr bool isErrorCode(Status code) {
        return ((static_cast<uint64_t>(code) >> 63) & 1) != 0U;
    }
} // namespace Uefi
