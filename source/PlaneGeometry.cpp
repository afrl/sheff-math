#include "simple_math/PlaneGeometry.h"

#include "simple_math/Utility.h"
#include "utility/Permutation.h"

namespace simple_math
{
/* Notes for coordinate systems
 *
 * From local coordinate to global:
 *     [ pv1_1  pv2_1  n_1 ]
 * R = [ pv1_2  pv2_2  n_2 ]
 *     [ pv1_3  pv2_3  n_3 ]
 *
 * From global to local:
 *       [ pv1_1  pv1_2  pv1_3 ]
 * R^T = [ pv2_1  pv2_2  pv2_3 ]
 *       [   n_1    n_2    n_3 ]
 *
 * 2D coordinate:
 * [ u ] = [ pv1_1  pv1_2  pv1_3 ] [ x ]
 * [ v ]   [ pv2_1  pv2_2  pv2_3 ] [ y ]
 *                                 [ z ]
 * Or u = plane_vector_1.dot(p)
 *    v = plane_vector_2.dot(p)
 */

std::vector<Vector2D> plane_2d_projection(const Vector3D plane_normal,
                                          const Vector3D point_on_plane,
                                          const std::vector<Vector3D>& points)
{
    // Establish a 2D coordinate system in the plane
    auto min_iter                = std::min_element(plane_normal.cbegin(), plane_normal.cend());
    auto min_normal_i            = std::distance(min_iter, plane_normal.cbegin());
    Vector3D plane_vector_1      = Vector3D::Zero();
    plane_vector_1(min_normal_i) = 1.0;
    Vector3D plane_vector_2      = plane_normal.cross(plane_vector_1);
    plane_vector_1               = plane_vector_2.cross(plane_normal);

    std::vector<Vector2D> points_2d(points.size());
    for (auto i : utility::indices(points))
    {
        auto p       = points[i];
        points_2d[i] = Vector2D(plane_vector_1.dot(p), plane_vector_2.dot(p));
    }

    return points_2d;
}

std::vector<Vector3D> reverse_2d_project(const Vector3D& plane_normal,
                                         const Vector3D& point_on_plane,
                                         const std::vector<Vector2D>& points_2d)
{
    // Note: must match plane_2d_projection
    // Establish a 2D coordinate system in the plane
    auto min_iter                = std::min_element(plane_normal.cbegin(), plane_normal.cend());
    auto min_normal_i            = std::distance(min_iter, plane_normal.cbegin());
    Vector3D plane_vector_1      = Vector3D::Zero();
    plane_vector_1(min_normal_i) = 1.0;
    Vector3D plane_vector_2      = plane_normal.cross(plane_vector_1);
    plane_vector_1               = plane_vector_2.cross(plane_normal);

    Matrix2x3 Q = Matrix2x3::Zero();
    Q.row(0)    = plane_vector_1;
    Q.row(1)    = plane_vector_2;

    std::vector<Vector3D> points_3d(points_2d.size());
    for (auto i : utility::indices(points_2d))
    {
        points_3d[i] = Q.transpose() * points_2d[i];
    }

    return points_3d;
}
}