#pragma once

#include "simple_text_input_protocol.h"

namespace Uefi {
    /// This class provides utility functions from reading and writing to the console.
    struct TextInputStream {
        using Key = SimpleTextInputProtocol::Key;
        SimpleTextInputProtocol* input;

        void setInput(SimpleTextInputProtocol& _input) { // NOLINT
            input = &_input;
        }

        Key readKeySync() {
            Key key;

            // TODO: something else than an infinite loop? What if input fails because of device errors?
            while (input->readKeyStroke(key) != Status::Success)
                ;

            return key;
        }
    };
} // namespace Uefi
