#pragma once

#include "EigenDefines.h"

#include "utility/Range.h"

namespace simple_math
{
	/// Returns if a < b while taking into account a numerical tolerance
	bool compare_points(const Point& a, const Point& b, const double tolerance);
	
	/// Returns an object that acts like a Vector3D without copying the memory
	/// from the given point.  Provides the Eigen interface for a Point.
	Eigen::Map<const Vector3D> point_as_vector(const Point& p);

    /// Convert vector to point
    static Point to_point(const Vector3D & v)
    {
        return { v[0], v[1], v[2] };
    }

    static std::string to_string(const simple_math::Point& point)
    {
        return "(" + std::to_string(point[0])
            + ", " + std::to_string(point[1])
            + ", " + std::to_string(point[2]) + ")";
    }

    template <typename T> inline constexpr
        int8_t sign(T x, std::false_type is_signed) {
        return T(0) < x;
    }

    template <typename T> inline constexpr
        int8_t sign(T x, std::true_type is_signed) {
        return (T(0) < x) - (x < T(0));
    }

    /// Returns the sign of a value (-1, 0, 1)
    template <typename T> inline constexpr
        int8_t sign(T x) {
        return sign(x, std::is_signed<T>());
    }

    template<typename ComplexVector>
    static std::pair<typename ComplexVector::value_type, typename ComplexVector::Index>
        find_max_real_part(const ComplexVector & vector)
    {
        typename ComplexVector::Index max_index = 0;
        typename ComplexVector::value_type max_value = vector[0];

        typename ComplexVector::Index curr_index = 0;
        for (auto i : utility::range_from_zero(vector.size()))
        {
            if (vector[i].real() > max_value.real())
            {
                max_index = curr_index;
                max_value = vector[i];
            }
            curr_index++;
        }

        return { max_value, max_index };
    }
};