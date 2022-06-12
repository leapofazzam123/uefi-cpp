# uefi-cpp
## A simple C++ library for interfacing with UEFI

> This is a fork of the original [UEFI++](https://github.com/GabrielMajeri/uefi-cpp) 

This project is a C++ wrapper for the UEFI specification, intended for people who write UEFI applications (OS loaders, shells, etc.)

This wrapper supports both 32 and 64 bit UEFI architectures.

**The current version is based on Version 2.9 of the UEFI specification, available [here](http://www.uefi.org/specifications).**

## Differences between the original library
- Merged UEFI and UEFI++ into one header library
- Renamed all camelCase function names to snake_case
- Added some missing or unfinished stuff
- Added CMake build support

## Example
Here's some code that uses the old, C-style API of the specification:

```c
system_table->ConOut->OutputString(system_table->ConOut, L"Hello UEFI!");
```

Here's some code that uses the new C++ API:

```c++
system_table.console_out.output_string(u"Hello UEFI!");
```

And here's one that's even more modern:

```c++
// Set the stream's output.
output.set_output(*system_table.console_out);

output << "Hello UEFI!";
```

Performance difference? Zero. Zero cost abstractions make this possible.

## Features
- Uses modern C++17 features.
- Mainly intended for UEFI applications, not tested for UEFI drivers.
- Relies on some C/C++ headers, but does not require a hosted standard library.

## Required software
* C++17 compatible compiler (Clang, GCC).
* A C++17 standard library (including the C headers). This wrapper only uses non-hosted functions from the standard lib, therefore it doesn't require a runtime. Some of the required headers:
  - `<cstdint>`: for cross-architecture integer types.
  - `<cstddef>`: for size_t.
  - `<type_traits>`: for some template metaprogramming and static assertions.
  - `<utility>`: for some utility functions (e.g. std::swap()).

* *(Optional)* QEMU with OVMF to be able to test UEFI apps in a virtual machine.
* *(Optional)* Doxygen to generate documentation.

## License

<a href="https://opensource.org/licenses/MIT">
  <img align="right" width="128" alt="MIT License" src="docs/mit.svg" />
</a>

The files included in this repository are licensed under the
[MIT License](https://opensource.org/licenses/MIT).

This means you are free to use the header files, even in a proprietary program,
as long as they are unmodified. You must open source any modifications to the
headers you use.

See the [license](LICENSE) for more information.

## Contributions
Anyone is welcome to contribute to this project. 

Please try to adhere to modern C++ coding style & guidelines and avoid C-style hacks. Use the **Issues** tab to submit bugs or feature requests, or submit improved code as **Pull requests**.
