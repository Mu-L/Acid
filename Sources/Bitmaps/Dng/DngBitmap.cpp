#include "DngBitmap.hpp"

#include <cstring>

#include <tinydng/tiny_dng.h>

#include "Files/Files.hpp"
#include "Maths/Time.hpp"

namespace acid {
void DngBitmap::Load(Bitmap *bitmap, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	auto fileLoaded = Files::Read(filename);

	if (!fileLoaded) {
		Log::Error("Bitmap could not be loaded: ", filename, '\n');
		return;
	}

	// TODO

#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " loaded in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}

void DngBitmap::Write(const Bitmap *bitmap, const std::filesystem::path &filename) {
#if defined(ACID_DEBUG)
	auto debugStart = Time::Now();
#endif

	// TODO

#if defined(ACID_DEBUG)
	Log::Out("Bitmap ", filename, " written in ", (Time::Now() - debugStart).AsMilliseconds<float>(), "ms\n");
#endif
}
}
