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

std::vector<Point2D> plane_2d_projection(const Vector3D plane_normal,
                                         const Vector3D point_on_plane,
                                         const std::vector<Vector3D>& points)
{
    // Establish a 2D coordinate system in the plane
    auto min_normal_i = std::distance(std::min_element(plane_normal.cbegin(), plane_normal.cend()),
                                      plane_normal.cend());
    Vector3D plane_vector_1      = Vector3D::Zero();
    plane_vector_1(min_normal_i) = 1.0;
    Vector3D plane_vector_2      = plane_normal.cross(plane_vector_1);

    std::vector<Point2D> points_2d(points.size());
    for (auto i : utility::indices(points))
    {
        auto p       = points[i];
        points_2d[i] = { plane_vector_1.dot(p), plane_vector_2.dot(p) };
    }

    return points_2d;
}
}