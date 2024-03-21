#pragma once

#include "Defines.h"
#include "EigenDefines.h"

#include <vector>

namespace simple_math
{
/// @brief Result of closest_point_on_line_segment
struct SegmentClosestPoint
{
    enum Type
    {
        INTERIOR,    ///< Closest point is interior to the line segment
        END_POINT_1, ///< Closest point is at first end point, p1
        END_POINT_2  ///< Closest point is at first end point, p2
    };
    /// Indicates if the closest point on the line segment is at the ends or within the segment.
    Type type;
    /// Closest distance to the line segment
    double distance;
    /// Coordinate of the closest point
    Point closest_point;
};

/// @brief Finds the closest point from p to the line segment defined by segment_p1 and segment_p2
SegmentClosestPoint closest_point_on_line_segment(const Point& p,
                                                  const Point& segment_p1,
                                                  const Point& segment_p2);

/// @brief Result of closest_point_on_lines
struct LineClosestPoint2D
{
    /// Closest distance to the infinite line (ignoring the segment bounds)
    double distance = -1.0;
    /// Coordinate of the closest point on the line to the given point
    Vector2D closest_point = Vector2D::Zero();
    /// Parametric distance (0 = end point #1, 1 = end point #2) along the infinite line
    /// that is the closest distance to the given point.
    double s = 0.0;
    /// Unit vector along the line segment, returned for convenience.
    Vector2D line_unit_vector = Vector2D::Zero();
};

/// @brief Finds the closest point from p to an infinite line defined by segment_p1 and segment_p2
LineClosestPoint2D closest_point_on_line_2D(const Vector2D& p,
                                            const Vector2D& segment_p1,
                                            const Vector2D& segment_p2);


struct CircleIntersection
{
    enum Type
    {
        NO_INTERSECTION,
        TANGENT,
        ONE_INTERSECT,
        TWO_INTERSECTS
    };
    Type type = CircleIntersection::NO_INTERSECTION;
    Point intersect_1;
    Point intersect_2;
};

/// @brief Computes the intersection between a circle and an infinite line
CircleIntersection intersect_circle_line(const double& r,
                                         const Point& center,
                                         const Point& line_p1,
                                         const Point& line_p2);

/// @brief Computes the intersection between a circle and finite line segment
CircleIntersection intersect_circle_line_segment(const double& r,
                                                 const Point& center,
                                                 const Point& segment_p1,
                                                 const Point& segment_p2);

/// @brief Determines if a point is inside a polygon using a ray-casting algorithm (note: if the
/// polygon self-intersects or has holes, it may be incorrect).
bool is_point_in_polygon(const Point& p, const std::vector<Point>& polygon_points);

/// @brief Finds and returns the permutation that would sort the list of points by their projection
///        onto a line defined by line_p1 and line_p2.
/// Note: you can use utility::apply_permutation to apply the permuation from utility/Permutation.h
std::vector<std::size_t> find_sort_for_points_on_line(const Point& line_p1,
                                                      const Point& line_p2,
                                                      const std::vector<Point>& points);
}