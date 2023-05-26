#include "simple_math/ConvexHull.h"

namespace simple_math
{

std::vector<std::size_t> convex_hull_for_3d_points_on_plane(
  const simple_math::Vector3D& plane_normal,
  const simple_math::Vector3D& plane_point,
  const std::vector<simple_math::Vector3D>& points)
{
    // Compute the lower bound of points - plane_point (min x, min y, min z)
    simple_math::Vector3D lower_bound;
    lower_bound.setConstant(std::numeric_limits<double>::max());
    for (auto i : utility::indices(points))
    {
        auto p = points[i];
        for (auto j : utility::range(3))
        {
            if (p(j) < lower_bound(j))
            {
                lower_bound(j) = p(j);
            }
        }
    }

    // Find point closest to the lower bound, which will be used to define our reference vector
    double smallest_dist            = std::numeric_limits<double>::max();
    std::size_t smallest_dist_index = 0;
    for (auto i : utility::indices(points))
    {
        auto p      = points[i];
        double dist = (p - lower_bound).norm();
        if (dist < smallest_dist)
        {
            smallest_dist       = dist;
            smallest_dist_index = i;
        }
    }

    // Define a reference vector:
    auto v      = points[smallest_dist_index] - plane_point;
    auto v_norm = v.norm();
    // Compute angle to every point in the cross-section relative to the reference vector
    std::vector<std::size_t> point_index_sorted_by_angle(points.size());
    std::iota(point_index_sorted_by_angle.begin(), point_index_sorted_by_angle.end(), 0);
    std::vector<double> angles(points.size(), 0.);
    for (auto i : utility::indices(points))
    {
        auto p      = points[i] - plane_point;
        auto p_norm = p.norm();

        auto w         = v.cross(p);
        auto cos_theta = v.dot(p) / v_norm / p_norm;
        // acos will error out if it outside the bounds of -1, 1
        // and it can be without machine precision
        if (cos_theta < -1.)
        {
            cos_theta = -1.;
        }
        else if (cos_theta > 1.)
        {
            cos_theta = 1.;
        }

        double theta = 0.;
        if (w.dot(plane_normal) >= 0)
        {
            theta = acos(cos_theta);
        }
        else
        {
            theta = 2. * EIGEN_PI - acos(cos_theta);
        }
        angles[i] = theta;
    }

    // Sort based on the angle
    std::sort(point_index_sorted_by_angle.begin(),
              point_index_sorted_by_angle.end(),
              [&angles](std::size_t i, std::size_t j) { return angles[i] < angles[j]; });
    assert(point_index_sorted_by_angle[0] == smallest_dist_index);

    // Find the points that would form the convex hull
    // Refer to https://lvngd.com/blog/convex-hull-graham-scan-algorithm-python/ for nice figures
    std::vector<std::size_t> convex_hull_point_indices = { point_index_sorted_by_angle[0],
                                                           point_index_sorted_by_angle[1] };
    for (auto i : utility::range(std::size_t(2), point_index_sorted_by_angle.size()))
    {
        convex_hull_point_indices.push_back(point_index_sorted_by_angle[i]);

        while (convex_hull_point_indices.size() > 2)
        {
            auto p1_index = convex_hull_point_indices[convex_hull_point_indices.size() - 3];
            auto p2_index = convex_hull_point_indices[convex_hull_point_indices.size() - 2];
            auto p3_index = convex_hull_point_indices[convex_hull_point_indices.size() - 1];
            auto w =
              (points[p2_index] - points[p1_index]).cross(points[p3_index] - points[p1_index]);
            auto w_dot_normal = w.dot(plane_normal);
            if (w_dot_normal < 0)
            {
                // Efficiently remove the second to last value
                convex_hull_point_indices[convex_hull_point_indices.size() - 2] = p3_index;
                convex_hull_point_indices.pop_back();
            }
            else
            {
                break;
            }
        }
    }
    // Correction to the reference above, if the last point referenced by
    // point_index_sorted_by_angle happens to break convex hull, it will not be removed.  This
    // checks that the condition of the cross-product aligning with the normal holds for the last
    // point as well.
    {
        auto p1_index = convex_hull_point_indices[convex_hull_point_indices.size() - 2];
        auto p2_index = convex_hull_point_indices[convex_hull_point_indices.size() - 1];
        auto p3_index = convex_hull_point_indices[0];
        auto w = (points[p2_index] - points[p1_index]).cross(points[p3_index] - points[p1_index]);
        auto w_dot_normal = w.dot(plane_normal);
        if (w_dot_normal < 0)
        {
            convex_hull_point_indices.pop_back();
        }
    }

    return convex_hull_point_indices;
}

}