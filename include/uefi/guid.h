#pragma once

#include <cstdint>

namespace Uefi
{
	/// Unique reference number used as an identifier for various protocols or for partition tables.
	/// See https://en.wikipedia.org/wiki/Globally_unique_identifier
	union Guid
	{
		/// Constructs a new GUID from some values (usually taken from the standard).
		constexpr Guid(std::uint32_t d1, std::uint16_t d2, std::uint16_t d3, const std::uint8_t (&d4)[8])
			: a{ d1, d2, d3, { /* Constexpr constructor must initialize all members. */ } }
		{
			// Can't use memset().
			for (std::uint8_t i = 0; i < 8; ++i)
				a.data4[i] = d4[i];
		}

		// This is how they are given in the standard.
		struct SeparateParts
		{
			std::uint32_t data1;
			std::uint16_t data2, data3;
			std::uint8_t data4[8];
		} a;

		// Used when comparing GUIDs.
		struct MergedGUID
		{
			std::uint64_t data1, data2;
		} b;
	};

	static_assert(sizeof(Guid) == 16, "GUIDs are supposed to be 128 bit big.");

	bool operator==(const Guid& lhs, const Guid& rhs)
	{
		return lhs.b.data1 == rhs.b.data2;
	}
}