#pragma once

#include "guid.h"
#include "signed_table.h"
#include "handle.h"
#include "time.h"
#include "boot_services.h"

namespace Uefi
{
	class RuntimeServices : public SignedTable<0x56524553544e5552>
	{
	public:
		Status get_time(Time& time, TimeCapabilities& capabilities)
		{
			return (this->*_get_time)(time, capabilities);
		}

		Status set_time(Time& time)
		{
			return (this->*_set_time)(time);
		}

		Status set_virtual_address_map(std::size_t map_size, std::size_t descriptor_size, std::uint32_t descriptor_version, BootServices::MemoryDescriptor& virtual_map)
		{
			return (this->*_set_virtual_address_map)(map_size, descriptor_size, descriptor_version, virtual_map);
		}

		enum class VariableAttributes : std::uint32_t
		{
			NonVolatile = 1,
			BootServiceAccess = 2,
			RuntimeAccess = 4,
			HardwareErrorRecord = 8,
			AuthenticatedWriteAccess = 16,
			TimeBasedAuthenticatedWriteAccess = 32,
			AppendWrite = 64,
			EnhancedAuthenticatedAccess = 128
		};

		Status get_variable(const char16_t* name, const Guid* guid, VariableAttributes& attributes, std::size_t& size, void*& data)
		{
			return (this->*_get_variable)(name, guid, attributes, size, data);
		}

		Status get_next_variable(std::size_t& name_size, const char16_t* name, const Guid* guid)
		{
			return (this->*_get_next_variable)(name_size, name, guid);
		}

		Status set_variable(const char16_t* name, const Guid* guid, VariableAttributes attributes, std::size_t size, void*& data)
		{
			return (this->*_set_variable)(name, guid, attributes, size, data);
		}

		enum class ResetType : std::uint32_t
		{
			Cold,
			Warm,
			Shutdown,
			PlatformSpecific
		};

		void reset(ResetType type, Status status, std::size_t size, void*& data)
		{
			return (this->*_reset)(type, status, size, data);
		}

		struct CapsuleHeader
		{
			Guid guid;
			std::uint32_t header_size;
			std::uint32_t flags;
			std::uint32_t image_size;
		};

		using PhysicalAddress = std::uint64_t;

		Status update_capsule(CapsuleHeader** header_array, std::size_t count, PhysicalAddress scatter_gather_list)
		{
			return (this->*_update_capsule)(header_array, count, scatter_gather_list);
		}

		Status query_capsule_capabilities(CapsuleHeader** header_array, std::size_t count, std::size_t& max_size, ResetType reset_type)
		{
			return (this->*_query_capsule_capabilities)(header_array, count, max_size, reset_type);
		}

		Status query_variable_info(VariableAttributes attributes, std::uint64_t& max_storage_size, std::uint64_t& remaining_storage_size, std::uint64_t& max_size)
		{
			return (this->*_query_variable_info)(attributes, max_storage_size, remaining_storage_size, max_size);
		}

	private:
		using _rs = RuntimeServices;

		decltype(&_rs::get_time) _get_time;
		decltype(&_rs::set_time) _set_time;

		[[maybe_unused]] void* _buf1[2];
	
		decltype(&_rs::set_virtual_address_map) _set_virtual_address_map;
		
		[[maybe_unused]] void* _buf2;
		
		decltype(&_rs::get_variable) _get_variable;
		decltype(&_rs::get_next_variable) _get_next_variable;
		decltype(&_rs::set_variable) _set_variable;
		
		[[maybe_unused]] void* _buf3;
		
		decltype(&_rs::reset) _reset;
		decltype(&_rs::update_capsule) _update_capsule;
		decltype(&_rs::query_capsule_capabilities) _query_capsule_capabilities;
		decltype(&_rs::query_variable_info) _query_variable_info;
	};
}
