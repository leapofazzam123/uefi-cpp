#pragma once

#include "table_header.h"

namespace Uefi {
    /// An UEFI table. It's recommended to inherit from this class if you plan to create your own table.
    struct Table {
        TableHeader header;
    };
} // namespace Uefi
