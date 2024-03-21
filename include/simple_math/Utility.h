#pragma once

#include "EigenDefines.h"
#include "utility/Range.h"

namespace simple_math
{
////////////////////////////////
// Utilities for scalars

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

/// Given a vector of complex numbers, returns the value and index (as a pair) of the entry with the
/// largest real part.
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

////////////////////////////////
// Utilities for 1-D points

/// Creates a linearly spaced list of double's between start and end with the specified number of
/// intervals.
/// @param endpoint - if true, then "end" is included. Otherwise, it is not.
std::vector<double> linspace(const double& start,
                             const double& end,
                             const int& n_intervals,
                             const bool endpoint = true);

/// Returns the mean of a list of doubles
double calculate_mean(const std::vector<double>& data);

/// Returns the standard deviation for a list of doubles
double calculate_std_dev(const std::vector<double>& data);

/// Returns an object that maps a std::vector to an Eigen vector
Eigen::Map<const DynamicDoubleVector> as_eigen_vector(const std::vector<double>& data);

////////////////////////////////
// Utilities for 2-D arrays

/// Convert Eigen vectors to a 2D point
template<typename DerivedType>
Point2D to_point2d(const Eigen::MatrixBase<DerivedType>& v)
{
    return { v[0], v[1] };
}

/// Returns the mean of a list of 2D vectors or points
Vector2D mean(const std::vector<Vector2D>& data);

////////////////////////////////
// Utilities for 3-D arrays

/// Returns if a < b while taking into account a numerical tolerance
bool compare_points(const Point& a, const Point& b, const double tolerance);

/// Returns the distance between two points.
double distance(const Point& a, const Point& b);

/// Creates a linearly spaced list of points between p1 and p2 with the specified number of
/// intervals.
std::vector<Point> linspace(const Point& p1,
                            const Point& p2,
                            const int& n_intervals,
                            const bool endpoint = true);

/// Returns the mean of a list of 3D vectors or points
Point mean(const std::vector<Point>& data);

/// Returns the mean of a list of 3D vectors or points
Vector3D mean(const std::vector<Vector3D>& data);

/// Returns an object that acts like a Vector3D without copying the memory
/// from the given point.  Provides the Eigen interface for a Point.
Eigen::Map<const Vector3D> point_as_vector(const Point& p);

/// Returns a Vector3D that is a copy of the given point.
Vector3D point_to_vector(const Point& p);

/// Returns an object that acts like a DynamicDoubleMatrix without copying the memory
/// from the given points.  Provides the Eigen interface for a list of Point objects.
Eigen::Map<const DynamicDoubleMatrix> points_as_matrix(const std::vector<Point>& points);

/// Convert Eigen vectors to point
template<typename DerivedType>
Point to_point(const Eigen::MatrixBase<DerivedType>& v)
{
    return { v[0], v[1], v[2] };
}

/// Returns a string in the form (x, y, z) given a point.
std::string to_string(const simple_math::Point& point);

namespace point_operators
{
/// @brief Multiply operator for a double with a point.  Returns a new scaled point.
static Point operator*(const double& d, const Point& p)
{
    return { d * p[0], d * p[1], d * p[2] };
}

/// @brief Division operator for a point by a scalar.  Returns a new scaled point.
static Point operator/(const Point& p, const double& d)
{
    return { p[0] / d, p[1] / d, p[2] / d };
}

/// @brief Addition operator for two points.  Returns a new point that has components that are the
/// sum of points a and b.
static Point operator+(const Point& a, const Point& b)
{
    return { a[0] + b[0], a[1] + b[1], a[2] + b[2] };
}

/// @brief Subtraction operator for two points.  Returns a new point that has components that are
/// the difference of points a and b.
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
} // point_operators
};