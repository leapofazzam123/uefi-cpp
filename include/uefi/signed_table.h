#pragma once

#include "table.h"

namespace Uefi
{
	/// This template struct is used to for UEFI tables that have signatures assigned by the UEFI standard.
	/// @tparam sgn The signature of this table.
	template <std::uint64_t sign>
	struct SignedTable : Table
	{
		/// The signature of this table, as specified by the standard.
		constexpr static Signature signature{ sign };

		/// Call this function to ensure a table instance matches its signature.
		/// @return True if the signature of this table instance matches the expected type signature.
		bool checkSignature() noexcept
		{
			return header.signature == signature;
		}
	};
}
