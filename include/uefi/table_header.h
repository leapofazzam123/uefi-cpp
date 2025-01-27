#pragma once

//#include "crc32.h"
#include "revision.h"
#include "signature.h"

namespace Uefi {
    using Crc32 = uint32_t;
    struct TableHeader {
        /// A 64-bit signature that identifies the type of table that follows.
        Signature signature;

        /// The revision of the EFI Specification to which this table conforms.
        Revision revision;

        /// The size, in bytes, of the entire table including this structure.
        uint32_t size;

        /// The 32-bit CRC for the entire table.
        /// This value is computed by setting this field to 0, and computing the 32-bit CRC for the whole table.
        Crc32 crc32;

        /// Reserved field that must be set to 0.
        uint32_t reserved;
    };
} // namespace Uefi
