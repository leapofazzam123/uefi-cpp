#pragma once

#include "detail/bit_flags.h"
#include "guid.h"
#include "non_copyable.h"
#include "status.h"
#include <cstddef>

namespace Uefi {
    template <size_t label_size = 128>
    struct FileSystemInfo {
        static constexpr Guid guid = {0x09576e93, 0x6d3f, 0x11d2, {0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b}};

        uint64_t size;
        bool read_only;
        uint64_t volume_size, free_space;
        uint32_t block_size;

        /// This field might not be large enough. Try with different sizes.
        char16_t volume_label[label_size];
    };

    template <size_t label_size = 128>
    struct FileSystemVolumeLabel {
        static constexpr Guid guid = {0xdb47d7d3, 0xfe81, 0x11d3, {0x9a, 0x35, 0x00, 0x90, 0x27, 0x3f, 0xC1, 0x4d}};

        char16_t volume_label[label_size];
    };

    enum class OpenMode : uint64_t {
        Create = 0x8000000000000000,
        Read = 1,
        Write = 2
    };

    UEFI_BIT_FLAGS(OpenMode);

    enum class FileAttributes : uint64_t {
        None = 0,
        ReadOnly = 1,
        Hidden = 2,
        System = 4,
        Reserved = 8,
        Directory = 0x10,
        Archive = 0x20,
        ValidAttr = 0x37
    };

    UEFI_BIT_FLAGS(FileAttributes);

    /// Provides file based access to supported file systems.
    class FileProtocol : private NonCopyable {
    public:
        uint64_t revision;

        /// @return Success The file was opened.
        /// @return NotFound The specified file could not be found on the device.
        /// @return NoMedia The device has no medium.
        /// @return MediaChanged The device has a different medium in it or the medium is no longer supported.
        /// @return DeviceError The device reported an error.
        /// @return VolumeCorrupted The file system structures are corrupted.
        /// @return WriteProtected An attempt was made to create a file, or open a file for write when the media is write-protected.
        /// @return AccessDenied The service denied access to the file.
        /// @return OutOfResources Not enough resources were available to open the file.
        /// @return VolumeFull The volume is full.
        Status open(FileProtocol*& new_handle, const char16_t* file_name, OpenMode open_mode, FileAttributes attributes) {
            return _open(new_handle, file_name, open_mode, attributes);
        }

        Status close() {
            return _close();
        }

        // EFI_FILE_DELETE Delete;

        Status read(size_t& buffer_size, void* buffer) {
            return _read(buffer_size, buffer);
        }

        Status write(size_t& buffer_size, const void* buffer) {
            return _write(buffer_size, buffer);
        }

        Status getPosition(uint64_t& position) {
            return _getPosition(position);
        }

        Status setPosition(uint64_t position) {
            return _setPosition(position);
        }

        Status getInfo(const Guid& info_type, size_t& buffer_size, void* buffer) {
            return _getInfo(info_type, buffer_size, buffer);
        }

        // EFI_FILE_SET_INFO SetInfo;
        // EFI_FILE_FLUSH Flush;

    private:
        Status _open(FileProtocol*&, const char16_t*, OpenMode, FileAttributes);
        Status _close();

        [[maybe_unused]] void* _buf1[1];

        Status _read(size_t&, void*);
        Status _write(size_t&, const void*);
        Status _getPosition(uint64_t&);
        Status _setPosition(uint64_t);
        Status _getInfo(const Guid&, size_t&, void*);
    };

    class FileProtocol2 : public FileProtocol {
    public:
        // EFI_FILE_OPEN_EX OpenEx;
        // EFI_FILE_READ_EX ReadEx;
        // EFI_FILE_WRITE_EX WriteEx;
        // EFI_FILE_FLUSH_EX FlushEx;

    private:
    };
} // namespace Uefi
