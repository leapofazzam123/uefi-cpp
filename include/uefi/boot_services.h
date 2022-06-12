#pragma once

#include "signed_table.h"
#include "status.h"
#include "guid.h"
#include "handle.h"
#include "detail/bit_flags.h"
#include "time.h"
#include "task_priority_level.h"
#include "memory_attribute.h"
#include "memory_type.h"

namespace Uefi
{
	class BootServices : public SignedTable<0x56524553544f4f42>
	{
	public:
		/// TaskPriorityServices
		/// @{
		/// Raises a task’s priority level and returns its previous level.
		/// @param new_tpl The new level to raise to.
		/// @return The old level.
		Tpl raise_tpl(Tpl new_tpl)
		{
			return _raise_tpl(new_tpl);
		}

		void restore_tpl(Tpl old_tpl)
		{
			_restore_tpl(old_tpl);
		}
		/// @}


		// Memory Services

		enum class AllocateType
		{
			/// Any available range of pages that satisfies the request.
			AnyPages,
			/// Any available range of pages whose uppermost address is less than or equal to the address pointed to by the input.
			MaxAddress,
			/// Allocate pages at the address pointed to by the input.
			Address,

			MaxAllocateType
		};

		using PhysicalAddress = std::uint64_t;
		using VirtualAddress = std::uint64_t;

		/// Allocates memory pages from the system.
		/// @return Success The requested pages were allocated.
		/// @return OutOfResources The pages could not be allocated.
		/// @return InvalidParameter Type is not a valid enum value.
		/// @return InvalidParameter MemoryType is in the range MaxMemoryType ... 0x6FFFFFFF.
		/// @return InvalidParameter MemoryType is PersistentMemory.
		/// @return NotFound The requested pages could not be found
		Status allocate_pages(AllocateType type, MemoryType mem_type, std::size_t pages, PhysicalAddress& memory)
		{
			return _allocate_pages(type, mem_type, pages, memory);
		}

		Status free_pages(PhysicalAddress memory, std::size_t pages)
		{
			return _free_pages(memory, pages);
		}

		/// Describes a region of memory.
		/// Note: firmware is free to have memory descriptors of different sizes. This structure can and will have extra
		/// variables at the end. Therefore, you should remember to check and use descriptorSize.
		struct MemoryDescriptor
		{
			/// The type of memory in this block.
			MemoryType type;

			/// This variable would have been added automatically by the compiler. It's here just to show there is a gap.
			std::uint32_t _pad;

			/// The start of this memory range.
			PhysicalAddress physical_start;

			/// This should / will always be 0 or some garbage value during startup. UEFI either uses identity mapping
			/// or disables paging (on 32 bit) on startup.
			VirtualAddress virtual_start;

			/// How many 4 KiB pages does this block contain.
			std::uint64_t pages_count;

			/// Attributes describe what settings the block CAN be configured for, not necessarily the current settings.
			MemoryAttribute attribute;
		};

		static_assert(sizeof(MemoryDescriptor) == 40);

		/// Returns the current memory map.
		/// @param[<65;44;28Main,out] map_size The size, in bytes, of the memory_map buffer.
		/// On output, it is the size of the buffer returned by the firmware if the buffer was large enough,
		/// or the size of the buffer needed to contain the map if the buffer was too small.
		/// @param[in,out] memory_map An array of MemoryDescriptors in which the firmware places the current memory map.
		/// @param[out] map_key The location in which firmware returns the key for the current memory map.
		/// @param[out] descriptor_size The size, in bytes, of a MemoryDescriptor.
		/// @param[out] descriptor_version The version number associated with the MemoryDescriptor.
		/// @return Success The memory map was returned in the buffer.
		/// @return BufferTooSmall The buffer was too small. The current buffer size needed to hold the memory map is returned in map_size.
		/// @return InvalidParameter The buffer is nullptr.
		Status get_memory_map(std::size_t& map_size, MemoryDescriptor* memory_map, std::size_t& map_key, std::size_t& descriptor_size, std::uint32_t& descriptor_version)
		{
			return _get_memory_map(map_size, memory_map, map_key, descriptor_size, descriptor_version);
		}

		/// Allocates pool memory.
		/// @param pool_type The type of pool to allocate.
		/// @param size How many bytes to allocate from the pool.
		/// @param[out] buffer Pointer to a pointer to the allocated buffer.
		/// @return Success The requested number of bytes was allocated.
		/// @return OutOfResources The pool requested could not be allocated.
		/// @return InvalidParameter The type is in the range MaxMemoryType .. 0x6FFFFFFF.
		/// @return InvalidParameter The type is PersistentMemory .
		/// @return InvalidParameter The buffer is nullptr.
		Status allocate_pool(MemoryType pool_type, std::size_t size, void** buffer)
		{
			return _allocate_pool(pool_type, size, buffer);
		}

		/// Returns pool memory to the system.
		Status free_pool(void* buffer)
		{
			return _free_pool(buffer);
		}

		//
		// Event & Timer Services
		//
		// EFI_CREATE_EVENT CreateEvent;
		// EFI_SET_TIMER SetTimer;
		// EFI_WAIT_FOR_EVENT WaitForEvent;
		// EFI_SIGNAL_EVENT SignalEvent;
		// EFI_CLOSE_EVENT CloseEvent;
		// EFI_CHECK_EVENT CheckEvent;

		//
		// Protocol Handler Services
		//
		// EFI_INSTALL_PROTOCOL_INTERFACE InstallProtocolInterface;
		// EFI_REINSTALL_PROTOCOL_INTERFACE ReinstallProtocolInterface;
		// EFI_UNINSTALL_PROTOCOL_INTERFACE UninstallProtocolInterface;

		/// Queries a handle to determine if it supports a specified protocol.
		/// @param[out] interface Pointer to the interface, or nullptr if handle does not support the interface.
		//[[deprecated("As of UEFI 1.10 you should use openProtocol()")]]
		Status handle_protocol(Handle handle, const Guid& protocol, void** interface)
		{
			return _handle_protocol(handle, protocol, interface);
		}

		// EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
		enum class LocateSearchType
		{
			AllHandles,
			ByRegisterNotify,
			ByProtocol
		};

		Status locate_handle(LocateSearchType search_type, const Guid* protocol, const void* search_key, std::size_t& handle_count, Handle*& buffer)
		{
			return _locate_handle(search_type, protocol, search_key, handle_count, buffer);
		}
		// EFI_LOCATE_DEVICE_PATH LocateDevicePath;
		// EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;

		//
		// Image Services
		//
		// EFI_IMAGE_LOAD LoadImage;
		// EFI_IMAGE_START StartImage;
		Status exit(Handle image_handle, std::size_t map_key)
		{
			return _exit(image_handle, map_key);
		}
		// EFI_IMAGE_UNLOAD UnloadImage;
		/// Terminates all boot services.
		Status exit_boot_services(Handle image_handle, std::size_t map_key)
		{
			return _exit_boot_services(image_handle, map_key);
		}

		//
		// Miscellaneous Services
		//
		// EFI_GET_NEXT_MONOTONIC_COUNT GetNextMonotonicCount;
		// EFI_STALL Stall;
		// EFI_SET_WATCHDOG_TIMER SetWatchdogTimer;

		// --> These only exist in UEFI 1.1+

		//
		// DriverSupport Services
		//
		// EFI_CONNECT_CONTROLLER ConnectController;
		// EFI_DISCONNECT_CONTROLLER DisconnectController;

		//
		// Open and Close Protocol Services
		//

		enum class OpenProtocolAttributes
		{
			ByHandleProtocol = 1,
			GetProtocol = 2,
			TestProtocol = 4,
			ByChildController = 8,
			ByDriver = 16,
			Exclusive = 32
		};

		/// Queries a handle to determine if it supports a specified protocol.
		/// If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent.
		Status open_protocol(Handle handle, const Guid& protocol_guid, void** interface, Handle agent, Handle controller, OpenProtocolAttributes attributes)
		{
			return _open_protocol(handle, protocol_guid, interface, agent, controller, attributes);
		}

		Status close_protocol(Handle handle, const Guid& protocol_guid, Handle agent, Handle controller)
		{
			return _close_protocol(handle, protocol_guid, agent, controller);
		}

		// EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;

		//
		// Library Services
		//
		// EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;

		Status locate_handle_buffer(LocateSearchType search_type, const Guid* protocol, const void* search_key, std::size_t& handle_count, Handle*& buffer)
		{
			return _locate_handle_buffer(search_type, protocol, search_key, handle_count, buffer);
		}

		Status locate_protocol(const Guid* protocol, void* registration, void** output)
		{
			return _locate_protocol(protocol, registration, output);
		}

		// EFI_LOCATE_PROTOCOL LocateProtocol;

		// EFI_INSTALL_MULTIPLE_PROTOCOL_INTERFACES InstallMultipleProtocolInterfaces;
		// EFI_UNINSTALL_MULTIPLE_PROTOCOL_INTERFACES UninstallMultipleProtocolInterfaces;

		//
		// 32-bit CRC Services
		//
		// EFI_CALCULATE_CRC32 CalculateCrc32;

		//
		// Miscellaneous Services

		/// Copies the contents of one buffer to another buffer.
		void copy_mem(void* destination, const void* source, std::size_t length)
		{
			return _copy_mem(destination, source, length);
		}

		/// Fills a buffer with a specified value.
		/// @param buffer Pointer to the buffer to fill.
		/// @param size Number of bytes in buffer to fill.
		/// @param value Value to fill buffer with.
		void set_mem(void* buffer, std::size_t size, std::uint8_t value)
		{
			return _set_mem(buffer, size, value);
		}

		// UEFI 2.0+
		// EFI_CREATE_EVENT_EX CreateEventEx;

	private:
		// Function pointers

		Tpl (*_raise_tpl)(Tpl);
		void (*_restore_tpl)(Tpl);


		Status (*_allocate_pages)(AllocateType, MemoryType, std::size_t, PhysicalAddress&);
		Status (*_free_pages)(PhysicalAddress, std::size_t);

		Status (*_get_memory_map)(std::size_t&, MemoryDescriptor*, std::size_t&, std::size_t&, std::uint32_t&);

		Status (*_allocate_pool)(MemoryType, std::size_t, void**);
		Status (*_free_pool)(void*);

		[[maybe_unused]] void* buf2[9];

		Status (*_handle_protocol)(Handle, const Guid&, void**);
		[[maybe_unused]] void* _reserved;

		[[maybe_unused]] void* buf3;

		Status (*_locate_handle)(LocateSearchType, const Guid*, const void*, std::size_t&, Handle*&);

		[[maybe_unused]] void* buf4[4];

		Status (*_exit)(Handle, std::size_t);

		[[maybe_unused]] void* buf5;

		Status (*_exit_boot_services)(Handle, std::size_t);

		[[maybe_unused]] void* buf6[5];

		Status (*_open_protocol)(Handle, const Guid&, void**, Handle, Handle, OpenProtocolAttributes);
		Status (*_close_protocol)(Handle, const Guid&, Handle, Handle);

		[[maybe_unused]] void* buf7[2];

		Status (*_locate_handle_buffer)(LocateSearchType, const Guid*, const void*, std::size_t&, Handle*&);
		Status (*_locate_protocol)(const Guid*, void*, void**);

		[[maybe_unused]] void* buf8[3];

		void (*_copy_mem)(void*, const void*, std::size_t);
		void (*_set_mem)(void*, std::size_t, std::uint8_t);

		[[maybe_unused]] void* buf9;
	};

	UEFI_BIT_FLAGS(BootServices::OpenProtocolAttributes);
}
