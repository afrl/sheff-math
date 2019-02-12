#pragma once

#include <array>

namespace simple_math
{
	enum class Frame : char {
		Reference,
		Current
	};

	using FloatType = double;
	using Point = std::array<FloatType, 3>;
}