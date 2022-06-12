#pragma once

#include <utility>
#include "console_color.h"
#include "simple_text_output_protocol.h"

namespace Uefi
{
	/// For UEFI newlines are in Carriage Return + Line Feed (Windows) format.
	constexpr auto new_line = u"\r\n";

	using TextColor = SimpleTextOutputProtocol::Attribute;

	/// Note: this "default color" isn't specified in the standard, but OVMF and mTextOutputStreamt computers use this as the default.
	constexpr TextColor default_color = { ForegroundColor::LightGray, BackgroundColor::Black };

	struct TextOutputStream
	{
		SimpleTextOutputProtocol* output;

		bool alpha;
		std::uint8_t base;

		char _padding[6];
		// Since static constructors require runtime support, it's better for users to just call initialize().
		void initialize()
		{
			alpha = true;
			base = 10;
		}

		void set_output(SimpleTextOutputProtocol& _output)
		{
			output = &_output;
		}

		void reset(bool extended_verification)
		{
			output->reset(extended_verification);
		}

		void clear()
		{
			output->clear_screen();
		}

		void set_number_base(std::uint8_t b)
		{
			base = b;
		}

		TextOutputStream& operator<<(const char16_t* msg)
		{
			this->output->output_string(msg);
			return *this;
		}

		TextOutputStream& operator<<(std::uint16_t n)
		{
			return *this << static_cast<std::uint64_t>(n);
		}

		TextOutputStream& operator<<(char16_t* msg)
		{
			return *this << static_cast<const char16_t*>(msg);
		}

		/// Prints a number in the specified base to `stream`.
		/// Base should be above 2, and no bigger than 36 (otherwise it would start running out of letters).
		void print_number(TextOutputStream& stream, std::uint64_t number)
		{
			auto base = stream.base;

			if (base < 2 || base > 36)
				return;

			// A 64 bit number can be up to 2^64, which means up to log2(2^64) = 64 digits in base 2.
			// In bigger bases then it is smaller.
			static char16_t buf[65];

			// Add some prefixes for various bases.
			switch (base)
			{
			case 2:
				stream << u"0b";
				break;

			case 8:
				stream << u"0";
				break;

			case 16:
				stream << u"0x";
				break;

			default:
				break;
			}

			int k = 0;

			if(number == 0)
			{
				buf[0] = '0';
				k = 1;
			}

			while (number)
			{
				std::uint8_t value = (number % base);

				// If it's a digit.
				if (value <= 9)
					buf[k++] = '0' + value;
				// Loop around to letters.
				else
					buf[k++] = 'A' + (value - 10);

				// Remove a digit.
				number /= base;
			}

			// TODO: is there no better solution than reversing?
			for (int i = 0; i < k / 2; ++i)
				std::swap(buf[i], buf[k - i - 1]);

			// Add a null terminator.
			buf[k] = 0;

			stream << buf;
		}

		TextOutputStream& operator<<(TextColor color)
		{
			this->output->set_attribute(color);
			return *this;
		}

		TextOutputStream& operator<<(bool value)
		{
			if (this->alpha)
				return *this << (value ? u"true" : u"false");
			else
				return *this << (value ? u"1" : u"0");
		}

		TextOutputStream& operator<<(std::uint64_t n)
		{
			print_number(*this, n);
			return *this;
		}

		TextOutputStream& operator<<(Status status)
		{
			std::uint64_t code = static_cast<std::uint64_t>(status);

			// Clear the high bit.
			code &= ~(1ull << 63);

			auto _base = this->base;

			this->set_number_base(10);
			*this << code;
			this->set_number_base(_base);

			return *this;
		}

		TextOutputStream& operator<<(const unsigned char* msg)
		{
			// TODO: bounds checking for buffer
			static char16_t buffer[512];

			auto ptr = buffer;

   	     while (*msg != 0)
   	         *ptr++ = *msg++;

			*ptr = 0;

			return *this << buffer;
		}

		TextOutputStream& operator<<(unsigned char* msg)
		{
			return *this << static_cast<const unsigned char*>(msg);
		}

		TextOutputStream& operator<<(const char* msg)
		{
			return *this << reinterpret_cast<const unsigned char*>(msg);
		}

		TextOutputStream& operator<<(char* msg)
		{
			return *this << reinterpret_cast<const unsigned char*>(msg);
		}
	};
}
