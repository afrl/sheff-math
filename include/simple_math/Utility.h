#pragma once

#include "EigenDefines.h"
#include "utility/Range.h"

namespace simple_math
{
namespace point_operators
{
/// @brief Multiply operator for a double with a point.  Returns a new scaled point.
static Point operator*(const double& d, const Point& p)
{
    return { d * p[0], d * p[1], d * p[2] };
}

/// @brief Addition operator for two points.  Returns a new point that has components that are the
/// sum of points a and b.
static Point operator+(const Point& a, const Point& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

/// @brief Subtraction operator for two points.  Returns a new point that has components that are the
/// difference of points a and b.
static Point operator-(const Point& a, const Point& b)
{
    return { a[0] - b[0], a[1] - b[1], a[2] - b[2] };
}

/// @brief Less than comparison for two points that sorts by x, then y, then z.
static bool operator<(const Point& a, const Point& b)
{
    return (a[0] < b[0]) || (a[0] == b[0] && a[1] < b[1]) ||
           (a[0] == b[0] && a[1] == b[1] && a[2] < b[2]);
}
}

/// Returns if a < b while taking into account a numerical tolerance
bool compare_points(const Point& a, const Point& b, const double tolerance);

static double distance(const Point& a, const Point& b)
{
    return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) +
                (a[2] - b[2]) * (a[2] - b[2]));
}

/// Returns an object that acts like a Vector3D without copying the memory
/// from the given point.  Provides the Eigen interface for a Point.
Eigen::Map<const Vector3D> point_as_vector(const Point& p);

/// Convert vector to point
static Point to_point(const Vector3D& v)
{
    return { v[0], v[1], v[2] };
}

template<typename EigenType>
Point to_point(const EigenType& v)
{
    return { v[0], v[1], v[2] };
}

static std::string to_string(const simple_math::Point& point)
{
    return "(" + std::to_string(point[0]) + ", " + std::to_string(point[1]) + ", " +
           std::to_string(point[2]) + ")";
}

namespace detail
{
template<typename T>
inline constexpr int8_t sign(T x, std::false_type is_signed)
{
    return T(0) < x;
}

template<typename T>
inline constexpr int8_t sign(T x, std::true_type is_signed)
{
    return (T(0) < x) - (x < T(0));
}

/// Sign function for unsigned types with approximate comparison
template<typename T>
inline constexpr int8_t sign(T x, std::false_type is_signed, const double tol)
{
    return tol < x;
}

template<typename T>
inline constexpr int8_t sign(T x, std::true_type is_signed, const double tol)
{
    return (tol < x) - (x < -tol);
}
}

/// Returns the sign of a value (-1, 0, 1)
template<typename T>
inline constexpr int8_t sign(T x)
{
    return detail::sign(x, std::is_signed<T>());
}

/// Returns the sign of a value (-1, 0, 1) with an approximate comparison
template<typename T>
inline constexpr int8_t sign(T x, const T tol)
{
    return detail::sign(x, std::is_signed<T>(), tol);
}

template<typename ComplexVector>
static std::pair<typename ComplexVector::value_type, typename ComplexVector::Index>
find_max_real_part(const ComplexVector& vector)
{
    typename ComplexVector::Index max_index      = 0;
    typename ComplexVector::value_type max_value = vector[0];

    typename ComplexVector::Index curr_index = 0;
    for (auto i : utility::range(vector.size()))
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