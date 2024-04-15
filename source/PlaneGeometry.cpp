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
 * [ w ]   [   n_1    n_2    n_3 ] [ z ]
 * Or u = plane_vector_1.dot(p)
 *    v = plane_vector_2.dot(p)
 *    w = plane_normal.dot(p)
 */

Project2DResult plane_2d_projection(const Vector3D plane_normal,
                                    const Vector3D point_on_plane,
                                    const std::vector<Vector3D>& points)
{
    // Establish a 2D coordinate system in the plane
    Eigen::Index min_normal_i = 0;
    for (auto i : utility::range(plane_normal.size()))
    {
        if (abs(plane_normal[i]) < abs(plane_normal[min_normal_i]))
        {
            min_normal_i = i;
        }
    }
    Vector3D plane_vector_1      = Vector3D::Zero();
    plane_vector_1(min_normal_i) = 1.0;
    Vector3D plane_vector_2      = plane_normal.cross(plane_vector_1);
    plane_vector_2.normalize();
    plane_vector_1 = plane_vector_2.cross(plane_normal);
    plane_vector_1.normalize();

    std::vector<Vector2D> points_2d(points.size());
    std::vector<double> w(points.size());
    for (auto i : utility::indices(points))
    {
        auto p       = points[i];
        points_2d[i] = Vector2D(plane_vector_1.dot(p), plane_vector_2.dot(p));
        w[i]         = plane_normal.dot(p);
    }

    return { points_2d, w };
}

std::vector<Vector3D> reverse_2d_project(const Vector3D& plane_normal,
                                         const Vector3D& point_on_plane,
                                         const std::vector<Vector2D>& points_2d,
                                         const std::vector<double>& w)
{
    // Note: must match plane_2d_projection
    // Establish a 2D coordinate system in the plane
    Eigen::Index min_normal_i = 0;
    for (auto i : utility::range(plane_normal.size()))
    {
        if (abs(plane_normal[i]) < abs(plane_normal[min_normal_i]))
        {
            min_normal_i = i;
        }
    }
    Vector3D plane_vector_1      = Vector3D::Zero();
    plane_vector_1(min_normal_i) = 1.0;
    Vector3D plane_vector_2      = plane_normal.cross(plane_vector_1);
    plane_vector_2.normalize();
    plane_vector_1 = plane_vector_2.cross(plane_normal);
    plane_vector_1.normalize();

    Matrix3D Q = Matrix3D::Zero();
    Q.col(0)   = plane_vector_1;
    Q.col(1)   = plane_vector_2;
    Q.col(2)   = plane_normal;

    std::vector<Vector3D> points_3d(points_2d.size());
    for (auto i : utility::indices(points_2d))
    {
        points_3d[i] = Q * Vector3D(points_2d[i][0], points_2d[i][1], w[i]);
    }

    return points_3d;
}
}