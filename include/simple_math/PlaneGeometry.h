#pragma once

#include "simple_math/EigenDefines.h"

namespace simple_math
{
struct Project2DResult
{
    /// @brief The 2D coordinates of each given 3D point
    std::vector<Vector2D> points_2d;
    /// @brief Third dimension of the projection for each given point, which is needed to reverse
    /// the projection.
    std::vector<double> w;
};

/// @brief Given a plane (defined by normal and a point), establishes a 2D coordinate system and
/// finds the projection of the 3D points onto the plane using the 2D coordinate system.
Project2DResult plane_2d_projection(const Vector3D plane_normal,
                                    const Vector3D point_on_plane,
                                    const std::vector<Vector3D>& points);

/// @brief Performs the reverse projection done by plane_2d_projection.
std::vector<Vector3D> reverse_2d_project(const Vector3D& plane_normal,
                                         const Vector3D& point_on_plane,
                                         const std::vector<Vector2D>& points_2d,
                                         const std::vector<double>& w);
}