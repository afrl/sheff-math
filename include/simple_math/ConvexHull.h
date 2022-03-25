#pragma once

#include "Defines.h"
#include "EigenDefines.h"

#include <vector>

namespace simple_math
{
/// Completes a convex hull for 2D points
/// @returns Ordered indices of points that form the convex hull
std::vector<std::size_t> convex_hull_2d(const std::vector<simple_math::Vector2D>& points);

/// Completes a convex hull for 3D points that all lie on a plane
/// @returns Ordered indices of points that form the convex hull within the plane
std::vector<std::size_t> convex_hull_for_3d_points_on_plane(
  const simple_math::Vector3D& plane_normal,
  const simple_math::Vector3D& plane_point,
  const std::vector<simple_math::Vector3D>& points);
}