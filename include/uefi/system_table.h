#pragma once

#include <cstddef>
#include <cstdint>

#include "boot_services.h"
#include "configuration_table.h"
#include "handle.h"
#include "runtime_services.h"
#include "signed_table.h"

namespace Uefi {
    class SimpleTextInputProtocol;
    class SimpleTextOutputProtocol;

    /// The UEFI System Table contains pointers to the runtime and boot services tables.
    /// After a call to ExitBootServices(), only the header, firmware, firmware, runtimeServices, numberOfTableEntries, and configurationTable are still vaild.
    struct SystemTable : public SignedTable<0x5453595320494249> {
        /// Information about the UEFI firmware.
        struct
        {
            /// A null-terminated string with the name of the vendor of the firmware.
            const char16_t* vendor;
            /// The version of the firmware.
            Revision revision;
        } firmware;

        Handle console_in_handle;
        SimpleTextInputProtocol* console_in;

        Handle console_out_handle;
        SimpleTextOutputProtocol* console_out;

        Handle standard_error_handle;
        SimpleTextOutputProtocol* standard_error;

        RuntimeServices* runtime_services;
        BootServices* boot_services;

        std::size_t table_entry_count;
        ConfigurationTable* configuration_table;
    };
} // namespace Uefi
