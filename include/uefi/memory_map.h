#pragma once

#include "boot_services.h"
#include <cstddef>
#include <cstdint>

namespace Uefi {
    struct MemoryMap {
        /// The size in bytes.
        size_t size;

        /// Size in bytes of each descriptor.
        /// The firmware is allowed to have extra information at the end of the descriptor,
        /// but it's backwards compatible.
        size_t entry_size;

        /// The current key. This is used by the firmware to determine if the map is current or not.
        /// Use this when calling exit_boot_services().
        size_t current_key;

        /// Pointer to an array of memory descriptors.
        /// Note: use operator[] to access these. They might not have the size of the structure.
        BootServices::MemoryDescriptor* descriptors;

        /// Number of descriptors in the array.
        [[nodiscard]] size_t getNumberOfEntries() const noexcept {
            return size / entry_size;
        }

        /// Accesses a memory region in the map.
        /// @param i The index of the region.
        /// @return The descriptor at that point.
        const BootServices::MemoryDescriptor& operator[](size_t i) const noexcept {
            return *reinterpret_cast<const BootServices::MemoryDescriptor*>(reinterpret_cast<const uint8_t*>(descriptors) + (i * entry_size));
        }
    };

    /// This function will retrieve the memory map. It determines how much memory is needed,
    /// asks UEFI to allocate a buffer,
    /// @param boot_services The boot services table.
    /// @return The memory map at the current time.
    /// @return The map will have size 0 if it fails.
    inline MemoryMap getMemoryMap(BootServices& boot_services) {
        // Get the memory map.
        BootServices::MemoryDescriptor* memory_map = nullptr;

        size_t map_size = 0;
        size_t key = 0;
        size_t descriptor_size = 0;
        uint32_t descriptor_version = 0;

        // Try getting the memory map.
        // No point in checking the status, since we know it will fail, because size is 0.
        boot_services.getMemoryMap(map_size, memory_map, key, descriptor_size, descriptor_version);

        // Allocating from the pool (might) add some extra descriptors.
        // TODO: is this extra enough?
        map_size += 4 * descriptor_size;

        // Now we know the needed size in mapSize.
        const auto status = boot_services.allocatePool(MemoryType::LoaderData, map_size, reinterpret_cast<void**>(&memory_map));

        // This could fail if there's not enough memory left.
        if (status != Status::Success)
            return {};

        // This cannot fail: we have allocated more than enough memory, and the buffer is not nullptr.
        boot_services.getMemoryMap(map_size, memory_map, key, descriptor_size, descriptor_version);

        return {map_size, descriptor_size, key, memory_map};
    }
} // namespace Uefi
