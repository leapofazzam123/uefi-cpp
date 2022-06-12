#pragma once

#include <cstdint>

#include "non_copyable.h"
#include "guid.h"
#include "status.h"

namespace Uefi
{
	class SimpleTextInputProtocol : private NonCopyable
	{
	public:
		static constexpr Guid guid = {0x387477c1,0x69c7,0x11d2, {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}};


		struct Key
		{
			std::uint16_t scan_code;

			/// The character that represents this key.
			/// 0 if not a printable character.
			char16_t unicode_char;
		};

		/// Resets the input device hardware.
		/// Clears the contents of any input queues and puts the input stream in a known empty state.
		/// @param extended_verification If true the driver may perform a more exhaustive verification of the device.
		/// @return Success: The device was reset.
		/// @return DeviceError: The device is not functioning correctly and could not be reset.
		[[gnu::ms_abi]]
		Status reset(bool extended_verification)
		{
			return (this->*_reset)(extended_verification);
		}

		/// Reads a keystroke from the input queue, if any.
		/// @param[out] key The structure to read keystroke data into.
		/// @return Sucess: The keystroke information was returned.
		/// @return NotReady: There was no keystroke data available.
		/// @return DeviceError: The keystroke information was not returned due to hardware errors.
		[[gnu::ms_abi]]
		Status read_key_stroke(Key& key)
		{
			return (this->*_read_key_stroke)(key);
		}

	private:
		using _sti = SimpleTextInputProtocol;
		decltype(&_sti::reset) _reset;
		decltype(&_sti::read_key_stroke) _read_key_stroke;
	};
}
