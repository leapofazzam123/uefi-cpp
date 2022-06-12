#pragma once

#include <cstdint>
#include <cstddef>
#include "boot_services.h"

namespace Uefi
{
	struct MemoryMap
	{
		/// The size in bytes.
		std::size_t size;

		/// Size in bytes of each descriptor.
		/// The firmware is allowed to have extra information at the end of the descriptor,
		/// but it's backwards compatible.
		std::size_t entry_size;

		/// The current key. This is used by the firmware to determine if the map is current or not.
		/// Use this when calling exit_boot_services().
		std::size_t current_key;

		/// Pointer to an array of memory descriptors.
		/// Note: use operator[] to access these. They might not have the size of the structure.
		BootServices::MemoryDescriptor* descriptors;

		/// Number of descriptors in the array.
		std::size_t get_number_of_entries() const noexcept
		{
			return size / entry_size;
		}

		/// Accesses a memory region in the map.
		/// @param i The index of the region.
		/// @return The descriptor at that point.
		const BootServices::MemoryDescriptor& operator[](std::size_t i) const noexcept
		{
			return *reinterpret_cast<const BootServices::MemoryDescriptor*>(reinterpret_cast<const std::uint8_t*>(descriptors) + (i * entry_size));
		}
	};

	/// This function will retrieve the memory map. It determines how much memory is needed,
	/// asks UEFI to allocate a buffer,
	/// @param boot_services The boot services table.
	/// @return The memory map at the current time.
	/// @return The map will have size 0 if it fails.
	inline MemoryMap get_memory_map(BootServices& boot_services)
	{
		// Get the memory map.
		BootServices::MemoryDescriptor* memory_map = nullptr;

		std::size_t map_size = 0, key, descriptor_size;
		std::uint32_t descriptor_version;

		// Try getting the memory map.
		// No point in checking the status, since we know it will fail, because size is 0.
		boot_services.get_memory_map(map_size, memory_map, key, descriptor_size, descriptor_version);

		// Allocating from the pool (might) add some extra descriptors.
		// TODO: is this extra enough?
		map_size += 4 * descriptor_size;

		// Now we know the needed size in mapSize.
		const auto status = boot_services.allocate_pool(MemoryType::LoaderData, map_size, reinterpret_cast<void**>(&memory_map));

		// This could fail if there's not enough memory left.
		if (status != Status::Success)
			return{ };

		// This cannot fail: we have allocated more than enough memory, and the buffer is not nullptr.
		boot_services.get_memory_map(map_size, memory_map, key, descriptor_size, descriptor_version);

		return{ map_size, descriptor_size, key, memory_map };
	}
}
