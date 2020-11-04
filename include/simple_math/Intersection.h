#pragma once

#include "simple_math/Utility.h"

#include <cinttypes>
#include <functional>

namespace simple_math
{
struct Intersection
{
    enum class Type : int8_t
    {
        INTERIOR,
        ENDPOINT
    };

    Point point;
    Type type;
};

using PolygonIndex = uint8_t;
struct SplitPolygonResult
{
    std::vector<PolygonIndex> points_on_negative_side;
    std::vector<PolygonIndex> pos_points;
};

/// @brief Finds intersections between a level-set (possibly non-linear) and a line segment.
/// @param signed_distance_function -
///    this can be non-linear but should only intersect the line segment at most once
/// @param p1 - one of the ends of the line segment
/// @param p1_value - the signed distance evaluated at p1 (allows cached values to be used)
/// @param p2 - the other end of the line segment
/// @param p2_value - the signed distance evaluated at p2 (allows cached values to be used)
///
/// Limitations:
/// 1) If the sign is different at p1 and p2, this assumes that there will only be one intersection
///    along the line segment.  If there was 3 or more intersections, only one will be found.
/// 2) If the end points of the segment have the same sign as the level-set, an intersection will
///    only be checked if at least one of the two points have a signed distance less than the length
///    of the line segment.
std::vector<Intersection> find_segment_level_set_intersection(
  const std::function<double(Point)> signed_distance_function,
  const Point& p1,
  const double& p1_value,
  const Point& p2,
  const double& p2_value);

/// @brief The same as the function above, but this internally calculates the signed distances at p1
/// and p2.
std::vector<Intersection> find_segment_level_set_intersection(
  const std::function<double(Point)> signed_distance_function,
  const Point& p1,
  const Point& p2);


}