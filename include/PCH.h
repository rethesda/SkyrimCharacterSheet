#pragma once

#include "RE/Skyrim.h"
#include "SKSE/SKSE.h"
#include <fmt/format.h>

#ifdef NDEBUG
#include <spdlog/sinks/basic_file_sink.h>
#else
#include <spdlog/sinks/msvc_sink.h>
#endif

#include "AutoTOML.hpp"


using namespace std;

using namespace literals;

namespace stl
{
	//using namespace SKSE::stl;
	using namespace SKSE::util;

	constexpr string_view safe_string(const char* a_str) { return a_str ? a_str : ""sv; }
}

namespace logger = SKSE::log;

#define DLLEXPORT __declspec(dllexport)

#include "Version.h"
#include "settings/settings.h"
#include "utils.h"
#include "constants.h"

#ifdef _MSC_VER
#undef GetObject
#endif