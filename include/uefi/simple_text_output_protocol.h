#pragma once

#include <cstddef>

#include "non_copyable.h"
#include "guid.h"
#include "status.h"

namespace Uefi
{
	// You can find these in the ConsoleColor header.
	enum class ForegroundColor : std::uint8_t;
	enum class BackgroundColor : std::uint8_t;

	class SimpleTextOutputProtocol : private NonCopyable
	{
	public:
		static constexpr Guid guid = {0x387477c2,0x69c7,0x11d2, {0x8e,0x39,0x00,0xa0,0xc9,0x69,0x72,0x3b}};

		/// Represents the console's current color settings.
		struct Attribute
		{
			ForegroundColor foreground : 4;

			// This should only be 3 bits wide, but we need to ensure the compiler sets the extra bit to 0.
			BackgroundColor background : 4;
		};

		/// Resets the text output device hardware.
		/// The cursor position is set to (0, 0).
		/// The screen is cleared to the default background color for the output device.
		/// @param extende_verification If set, the device may run exhaustive error checks.
		/// @return Success The text output device was reset.
		/// @return DeviceError The text output device is not functioning correctly and could not be reset.
		Status reset(bool extended_verification)
		{
			return (this->*_reset)(extended_verification);
		}

		/// Writes a string to the output device.
		/// @param string The null-terminated string to print.
		/// @return Success The string was output to the device.
		/// @return DeviceError The device reported an error while attempting to output the text.
		/// @return Unsupported The output device’s mode is not currently in a defined text mode.
		/// @return WarnUnknownGlyph This warning code indicates that some of the characters in the string could not be rendered and were skipped.
		Status output_string(const char16_t* string)
		{
			return (this->*_output_string)(string);
		}

		// TODO: document these functions
		Status test_string(const char16_t* string)
		{
			return (this->*_test_string)(string);
		}

		Status query_mode(std::size_t mode_number, std::size_t& columns, std::size_t& rows)
		{
            return (this->*_query_mode)(mode_number, columns, rows);
		}

		/// Sets the output device to a specified mode.
		/// On success the device is in the geometry for the requested mode,
		/// and the device has been cleared to the current background color with the cursor at (0,0).
		/// @return Success The requested text mode was set.
		/// @return DeviceError The device had an error and could not complete the request.
		/// @return Unsupported The mode number was not valid.
		Status set_mode(std::size_t mode_number)
		{
			return (this->*_set_mode)(mode_number);
		}

		/// Sets the background and foreground colors for the output_string() and clear_screen() functions.
		/// @param attribute The attribute to set (foreground and background color).
		/// @return Success The requested attributes were set.
		/// @return DeviceError The device had an error and could not complete the request.
		Status set_attribute(Attribute attribute)
		{
			return (this->*_set_attribute)(attribute);
		}

		/// Clears the output device display to the currently selected background color.
		/// @return Success The operation completed successfully.
		/// @return DeviceError The device had an error and could not complete the request.
		/// @return Unsupported The output device is not in a valid text mode.
		Status clear_screen()
		{
			return (this->*_clear_screen)();
		}

	private:
		// Function pointers.
		using _stp = SimpleTextOutputProtocol;
		decltype(&_stp::reset) _reset;
		decltype(&_stp::output_string) _output_string;
		decltype(&_stp::test_string) _test_string;
		decltype(&_stp::query_mode) _query_mode;
		decltype(&_stp::set_mode) _set_mode;
		decltype(&_stp::set_attribute) _set_attribute;
		decltype(&_stp::clear_screen) _clear_screen;
	};
}
