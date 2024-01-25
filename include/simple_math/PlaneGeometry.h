#pragma once

#include "simple_math/EigenDefines.h"

namespace simple_math
{
/// @brief Given a plane (defined by normal and a point), establishes a 2D coordinate system and
/// finds the projection of the 3D points onto the plane using the 2D coordinate system.
std::vector<Point2D> plane_2d_projection(const Vector3D plane_normal,
                                         const Vector3D point_on_plane,
                                         const std::vector<Vector3D>& points);
}