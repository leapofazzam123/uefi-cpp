#pragma once

namespace Uefi
{
	/// Represents an opaque handle, usually allocated by the UEFI boot loader.
	using Handle = struct { } *;
}
