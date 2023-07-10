#pragma once

#include <array>
#include <cstdint>

namespace simple_math
{
enum class Frame : char
{
    Reference,
    Current
};

using FloatType       = double;
using Point           = std::array<FloatType, 3>;
using Point2D         = std::array<FloatType, 2>;
using QuadratureIndex = uint16_t;
using QuadratureOrder = uint8_t;
}