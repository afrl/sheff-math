#pragma once

#include <array>
#include <cstdint>

namespace simple_math
{
	enum class Frame : char {
		Reference,
		Current
	};

	using FloatType = double;
	using Point = std::array<FloatType, 3>;
    using QuadratureIndex = uint8_t;
    using QuadratureOrder = uint8_t;
}