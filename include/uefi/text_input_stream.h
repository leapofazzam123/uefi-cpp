#pragma once

#include "simple_text_input_protocol.h"

namespace Uefi
{
	/// This class provides utility functions from reading and writing to the console.
	struct TextInputStream
	{
		using Key = SimpleTextInputProtocol::Key;
		SimpleTextInputProtocol* input;

		void set_input(SimpleTextInputProtocol& _input)
		{
			input = &_input;
		}

		Key read_key_sync()
		{
			Key key;

			// TODO: something else than an infinite loop? What if input fails because of device errors?
			while(input->read_key_stroke(key) != Status::Success);

			return key;
		}
	};
}
