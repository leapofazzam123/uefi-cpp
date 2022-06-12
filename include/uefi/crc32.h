#pragma once

#include <cstdint>
#include <cstddef>
#include "table.h"

namespace Uefi
{
	/// UEFI uses a standard CCITT32 CRC algorithm with a seed polynomial value of 0x04C11DB7 for its CRC calculations.
	//using Crc32 = std::uint32_t;

	struct Table;

	/// Algorithm to calculate CCITT32 for a UEFI structure.
	/// Original: http://stackoverflow.com/a/26051190
	/// Adapted the original to C++ and turned table generator into a constexpr function.
	/// @param data Pointer to the start of the structure.
	/// @param size The size in bytes of the structure.
	/// @return The calculated CRC32.
	Crc32 calculate_crc32(const void* data, std::size_t size) noexcept
	{
		constexpr static struct Crc32Table
		{
			std::uint32_t data[256] { };

			constexpr Crc32Table()
			{
				// 0xEDB88320 is 0x04C11DB7 but with changed endianess.
				constexpr std::uint32_t polynomial = 0xEDB88320;

				for(std::uint32_t i = 0; i < 256; ++i)
				{
					std::uint32_t remainder = i;

					for (std::uint32_t bit = 8; bit > 0; --bit)
						if (remainder & 1)
							remainder = (remainder >> 1) ^ polynomial;
						else
							remainder = (remainder >> 1);

					data[i] = remainder;
				}
			}

			std::uint32_t operator[](std::uint8_t i) const
			{
				return data[i];
			}

		} crc_table;

		Crc32 crc = 0xFF'FF'FF'FF;

		// Pointer to go through the data.
		auto ptr = reinterpret_cast<const std::uint8_t*>(data);

		for(std::size_t i = 0; i < size; ++i, ++ptr)
			crc = crc_table[*ptr ^ (crc & 0xFF)] ^ (crc >> 8);

		return ~crc;
	}

	/// Calculates the CRC32 of an UEFI table.
	/// @param table The table to calculate.
	/// @return The CRC of the table.
    Crc32 calculate_crc32(Table& table) noexcept
	{
		// CRC must be set to 0 before calculating.
		const auto _crc = table.header.crc32;

		table.header.crc32 = 0;

		const auto result = calculate_crc32(&table, table.header.size);

		// Reset the original CRC.
		table.header.crc32 = _crc;

		return result;
    }

	/// Verifies an UEFI table's integrity.
	/// @param table The table to check.
	/// @return True if the table's specified CRC value matches the table's specified CRC.
	bool does_crc32_match(Table& table) noexcept
	{
        return table.header.crc32 == calculate_crc32(table);
	}
}
