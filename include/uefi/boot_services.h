#pragma once

#include "detail/bit_flags.h"
#include "guid.h"
#include "handle.h"
#include "memory_attribute.h"
#include "memory_type.h"
#include "signed_table.h"
#include "status.h"
#include "task_priority_level.h"
#include <ctime>

namespace Uefi {
    class BootServices : public SignedTable<0x56524553544f4f42> {
    public:
        /// TaskPriorityServices
        /// @{
        /// Raises a task’s priority level and returns its previous level.
        /// @param new_tpl The new level to raise to.
        /// @return The old level.
        Tpl raiseTpl(Tpl new_tpl) {
            return _raiseTpl(new_tpl);
        }

        void restoreTpl(Tpl old_tpl) {
            _restoreTpl(old_tpl);
        }
        /// @}

        // Memory Services

        enum class AllocateType {
            /// Any available range of pages that satisfies the request.
            AnyPages,
            /// Any available range of pages whose uppermost address is less than or equal to the address pointed to by the input.
            MaxAddress,
            /// Allocate pages at the address pointed to by the input.
            Address,

            MaxAllocateType
        };

        using PhysicalAddress = uint64_t;
        using VirtualAddress = uint64_t;

        /// Allocates memory pages from the system.
        /// @return Success The requested pages were allocated.
        /// @return OutOfResources The pages could not be allocated.
        /// @return InvalidParameter Type is not a valid enum value.
        /// @return InvalidParameter MemoryType is in the range MaxMemoryType ... 0x6FFFFFFF.
        /// @return InvalidParameter MemoryType is PersistentMemory.
        /// @return NotFound The requested pages could not be found
        Status allocatePages(AllocateType type, MemoryType mem_type, size_t pages, PhysicalAddress& memory) {
            return _allocatePages(type, mem_type, pages, memory);
        }

        Status freePages(PhysicalAddress memory, size_t pages) {
            return _freePages(memory, pages);
        }

        /// Describes a region of memory.
        /// Note: firmware is free to have memory descriptors of different sizes. This structure can and will have extra
        /// variables at the end. Therefore, you should remember to check and use descriptorSize.
        struct MemoryDescriptor {
            /// The type of memory in this block.
            MemoryType type;

            /// This variable would have been added automatically by the compiler. It's here just to show there is a gap.
            uint32_t _pad;

            /// The start of this memory range.
            PhysicalAddress physical_start;

            /// This should / will always be 0 or some garbage value during startup. UEFI either uses identity mapping
            /// or disables paging (on 32 bit) on startup.
            VirtualAddress virtual_start;

            /// How many 4 KiB pages does this block contain.
            uint64_t pages_count;

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
        Status getMemoryMap(size_t& map_size, MemoryDescriptor* memory_map, size_t& map_key, size_t& descriptor_size, uint32_t& descriptor_version) {
            return _getMemoryMap(map_size, memory_map, map_key, descriptor_size, descriptor_version);
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
        Status allocatePool(MemoryType pool_type, size_t size, void** buffer) {
            return _allocatePool(pool_type, size, buffer);
        }

        /// Returns pool memory to the system.
        Status freePool(void* buffer) {
            return _freePool(buffer);
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
        Status handleProtocol(Handle handle, const Guid& protocol, void** interface) {
            return _handleProtocol(handle, protocol, interface);
        }

        // EFI_REGISTER_PROTOCOL_NOTIFY RegisterProtocolNotify;
        enum class LocateSearchType {
            AllHandles,
            ByRegisterNotify,
            ByProtocol
        };

        Status locateHandle(LocateSearchType search_type, const Guid* protocol, const void* search_key, size_t& handle_count, Handle*& buffer) {
            return _locateHandle(search_type, protocol, search_key, handle_count, buffer);
        }
        // EFI_LOCATE_DEVICE_PATH LocateDevicePath;
        // EFI_INSTALL_CONFIGURATION_TABLE InstallConfigurationTable;

        //
        // Image Services
        //
        // EFI_IMAGE_LOAD LoadImage;
        // EFI_IMAGE_START StartImage;
        Status exit(Handle image_handle, size_t map_key) {
            return _exit(image_handle, map_key);
        }
        // EFI_IMAGE_UNLOAD UnloadImage;
        /// Terminates all boot services.
        Status exitBootServices(Handle image_handle, size_t map_key) {
            return _exitBootServices(image_handle, map_key);
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

        enum class OpenProtocolAttributes {
            ByHandleProtocol = 1,
            GetProtocol = 2,
            TestProtocol = 4,
            ByChildController = 8,
            ByDriver = 16,
            Exclusive = 32
        };

        /// Queries a handle to determine if it supports a specified protocol.
        /// If the protocol is supported by the handle, it opens the protocol on behalf of the calling agent.
        Status openProtocol(Handle handle, const Guid& protocol_guid, void** interface, Handle agent, Handle controller, OpenProtocolAttributes attributes) {
            return _openProtocol(handle, protocol_guid, interface, agent, controller, attributes);
        }

        Status closeProtocol(Handle handle, const Guid& protocol_guid, Handle agent, Handle controller) {
            return _closeProtocol(handle, protocol_guid, agent, controller);
        }

        // EFI_OPEN_PROTOCOL_INFORMATION OpenProtocolInformation;

        //
        // Library Services
        //
        // EFI_PROTOCOLS_PER_HANDLE ProtocolsPerHandle;

        Status locateHandleBuffer(LocateSearchType search_type, const Guid* protocol, const void* search_key, size_t& handle_count, Handle*& buffer) {
            return _locateHandleBuffer(search_type, protocol, search_key, handle_count, buffer);
        }

        Status locateProtocol(const Guid* protocol, void* registration, void** output) {
            return _locateProtocol(protocol, registration, output);
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
        void copyMem(void* destination, const void* source, size_t length) {
            return _copyMem(destination, source, length);
        }

        /// Fills a buffer with a specified value.
        /// @param buffer Pointer to the buffer to fill.
        /// @param size Number of bytes in buffer to fill.
        /// @param value Value to fill buffer with.
        void setMem(void* buffer, size_t size, uint8_t value) {
            return _setMem(buffer, size, value);
        }

        // UEFI 2.0+
        // EFI_CREATE_EVENT_EX CreateEventEx;

    private:
        // Function pointers

        Tpl (*_raiseTpl)(Tpl);
        void (*_restoreTpl)(Tpl);

        Status (*_allocatePages)(AllocateType, MemoryType, size_t, PhysicalAddress&);
        Status (*_freePages)(PhysicalAddress, size_t);

        Status (*_getMemoryMap)(size_t&, MemoryDescriptor*, size_t&, size_t&, uint32_t&);

        Status (*_allocatePool)(MemoryType, size_t, void**);
        Status (*_freePool)(void*);

        [[maybe_unused]] void* _buf2[9];

        Status (*_handleProtocol)(Handle, const Guid&, void**);
        [[maybe_unused]] void* _reserved;

        [[maybe_unused]] void* _buf3;

        Status (*_locateHandle)(LocateSearchType, const Guid*, const void*, size_t&, Handle*&);

        [[maybe_unused]] void* _buf4[4];

        Status (*_exit)(Handle, size_t);

        [[maybe_unused]] void* _buf5;

        Status (*_exitBootServices)(Handle, size_t);

        [[maybe_unused]] void* _buf6[5];

        Status (*_openProtocol)(Handle, const Guid&, void**, Handle, Handle, OpenProtocolAttributes);
        Status (*_closeProtocol)(Handle, const Guid&, Handle, Handle);

        [[maybe_unused]] void* _buf7[2];

        Status (*_locateHandleBuffer)(LocateSearchType, const Guid*, const void*, size_t&, Handle*&);
        Status (*_locateProtocol)(const Guid*, void*, void**);

        [[maybe_unused]] void* _buf8[3];

        void (*_copyMem)(void*, const void*, size_t);
        void (*_setMem)(void*, size_t, uint8_t);

        [[maybe_unused]] void* _buf9;
    };

    UEFI_BIT_FLAGS(BootServices::OpenProtocolAttributes);
} // namespace Uefi
