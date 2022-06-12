#pragma once

#include <cstdint>

namespace Uefi
{
	struct Time
	{
		std::uint16_t year;
		std::uint8_t month;
		std::uint8_t day;
		std::uint8_t hour;
		std::uint8_t minute;
		std::uint8_t second;
		std::uint32_t nanosecond;
		int16_t timezone = 0x7ff;

		std::uint8_t _pad;
		
		std::uint8_t adjust_daylight;
		std::uint8_t in_daylight;
	};

	struct TimeCapabilities
	{
		std::uint32_t resolution;
		std::uint32_t accuracy;
		bool sets_to_zero;
	};
}
