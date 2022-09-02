#pragma once

#include "boot_services.h"
#include "guid.h"
#include "handle.h"
#include "signed_table.h"
#include "time.h"

namespace Uefi {
    class RuntimeServices : public SignedTable<0x56524553544e5552> {
    public:
        Status getTime(Time& time, TimeCapabilities& capabilities) {
            return _getTime(time, capabilities);
        }

        Status setTime(Time& time) {
            return _setTime(time);
        }

        Status setVirtualAddressMap(size_t map_size, size_t descriptor_size, uint32_t descriptor_version, BootServices::MemoryDescriptor& virtual_map) {
            return _setVirtualAddressMap(map_size, descriptor_size, descriptor_version, virtual_map);
        }

        enum class VariableAttributes : uint32_t {
            NonVolatile = 1,
            BootServiceAccess = 2,
            RuntimeAccess = 4,
            HardwareErrorRecord = 8,
            AuthenticatedWriteAccess = 16,
            TimeBasedAuthenticatedWriteAccess = 32,
            AppendWrite = 64,
            EnhancedAuthenticatedAccess = 128
        };

        Status getVariable(const char16_t* name, const Guid* guid, VariableAttributes& attributes, size_t& size, void*& data) {
            return _getVariable(name, guid, attributes, size, data);
        }

        Status getNextVariable(size_t& name_size, const char16_t* name, const Guid* guid) {
            return _getNextVariable(name_size, name, guid);
        }

        Status setVariable(const char16_t* name, const Guid* guid, VariableAttributes attributes, size_t size, void*& data) {
            return _setVariable(name, guid, attributes, size, data);
        }

        enum class ResetType : uint32_t {
            Cold,
            Warm,
            Shutdown,
            PlatformSpecific
        };

        void reset(ResetType type, Status status, size_t size, void*& data) {
            return _reset(type, status, size, data);
        }

        struct CapsuleHeader {
            Guid guid;
            uint32_t header_size;
            uint32_t flags;
            uint32_t image_size;
        };

        using PhysicalAddress = uint64_t;

        Status updateCapsule(CapsuleHeader** header_array, size_t count, PhysicalAddress scatter_gather_list) {
            return _updateCapsule(header_array, count, scatter_gather_list);
        }

        Status queryCapsuleCapabilities(CapsuleHeader** header_array, size_t count, size_t& max_size, ResetType reset_type) {
            return _queryCapsuleCapabilities(header_array, count, max_size, reset_type);
        }

        Status queryVariableInfo(VariableAttributes attributes, uint64_t& max_storage_size, uint64_t& remaining_storage_size, uint64_t& max_size) {
            return _queryVariableInfo(attributes, max_storage_size, remaining_storage_size, max_size);
        }

    private:
        Status _getTime(Time&, TimeCapabilities&);
        Status _setTime(Time&);

        [[maybe_unused]] void* _buf1[2];

        Status _setVirtualAddressMap(size_t, size_t, uint32_t, BootServices::MemoryDescriptor&);

        [[maybe_unused]] void* _buf2;

        Status _getVariable(const char16_t*, const Guid*, VariableAttributes&, size_t&, void*&);
        Status _getNextVariable(size_t&, const char16_t*, const Guid*);
        Status _setVariable(const char16_t*, const Guid*, VariableAttributes, size_t, void*&);

        [[maybe_unused]] void* _buf3;

        void _reset(ResetType, Status, size_t, void*&);
        Status _updateCapsule(CapsuleHeader**, size_t, PhysicalAddress);
        Status _queryCapsuleCapabilities(CapsuleHeader**, size_t, size_t&, ResetType);
        Status _queryVariableInfo(VariableAttributes, uint64_t&, uint64_t&, uint64_t&);
    };
} // namespace Uefi
