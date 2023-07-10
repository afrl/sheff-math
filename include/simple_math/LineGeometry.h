#pragma once

#include "Defines.h"

#include <vector>

namespace simple_math
{
struct SegmentClosestPoint
{
    enum Type
    {
        INTERIOR,    ///< Closest point is interior to the line segment
        END_POINT_1, ///< Closest point is at first end point, p1
        END_POINT_2  ///< Closest point is at first end point, p2
    };
    Type type;
    double distance;
    Point closest_point;
};

/// @brief Finds the closest point from p to the line segment defined by segment_p1 and segment_p2
SegmentClosestPoint closest_point_on_line_segment(const Point& p,
                                                  const Point& segment_p1,
                                                  const Point& segment_p2);

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