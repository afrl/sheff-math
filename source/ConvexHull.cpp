#include "simple_math/ConvexHull.h"

#include "simple_math/external/nanoflann.h"

namespace simple_math
{
using KDTreeType = nanoflann::KDTreeEigenMatrixAdaptor<Eigen::Map<const DynamicDoubleMatrix>,
                                                       3 /* 3D points */,
                                                       nanoflann::metric_L2 /* use L2 norm */,
                                                       true /* row-major format */>;

/// @brief Compute the lower bound of points = (min x, min y, min z)
Vector3D find_lower_bound(const std::vector<Vector3D>& points)
{
    Vector3D lower_bound;
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
    return lower_bound;
}

/// @brief Returns the lists of indices for points that would sort them by the angle based on the
/// cross-products relative to a given vector.
/// @return
std::vector<std::size_t> find_permutation_sorted_by_angle(const std::vector<Vector3D>& points,
                                                          const Vector3D& plane_normal,
                                                          const Vector3D& plane_point,
                                                          const Vector3D& reference_v)
{
    // Define a reference vector:
    auto reference_v_norm = reference_v.norm();
    // Compute angle to every point in the cross-section relative to the reference vector
    std::vector<std::size_t> point_index_sorted_by_angle(points.size());
    std::iota(point_index_sorted_by_angle.begin(), point_index_sorted_by_angle.end(), 0);
    std::vector<double> angles(points.size(), 0.);
    for (auto i : utility::indices(points))
    {
        auto p      = points[i] - plane_point;
        auto p_norm = p.norm();

        auto w         = reference_v.cross(p);
        auto cos_theta = reference_v.dot(p) / reference_v_norm / p_norm;
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

    return point_index_sorted_by_angle;
}

std::vector<std::size_t> concave_hull_for_3d_points_on_plane(const Vector3D& plane_normal,
                                                             const Vector3D& plane_point,
                                                             const std::vector<Vector3D>& points,
                                                             const int& k_a)
{
    _UTIL_ASSERT(points.size() > 2, "A concave hull cannot be found for < 3 points.");

    // "k" will be the number of nearest neighbors to use within the search later in the algorithm,
    // but we need to make sure the value meets some constraints.
    auto k = k_a;
    if (k < 3)
    {
        k = 3;
    }
    if (k > points.size() - 1)
    {
        k = points.size() - 1;
    }

    // Compute the lower bound of points - plane_point (min x, min y, min z)
    Vector3D lower_bound = find_lower_bound(points);

    // Create a KD tree of the points
    Eigen::Map<const DynamicDoubleMatrix> points_map(points[0].data(), points.size(), 3);
    KDTreeType kd_tree(3, points_map);

    // Find point closest to the lower bound, which will be used to define our reference vector
    Eigen::Index point_nearest_lb       = 0;
    double point_nearest_lb_sq_distance = 0.0;
    kd_tree.index->knnSearch(lower_bound.data(),
                             1,
                             &point_nearest_lb,
                             &point_nearest_lb_sq_distance);

    // Sort based on the angle
    auto reference_v = points[point_nearest_lb] - plane_point;
    auto point_index_sorted_by_angle =
      find_permutation_sorted_by_angle(points, plane_normal, plane_point, reference_v);
    assert(point_index_sorted_by_angle[0] == point_nearest_lb);

    std::vector<std::size_t> concave_hull_point_indices = { point_index_sorted_by_angle[0] };

    // See http://repositorium.sdum.uminho.pt/bitstream/1822/6429/1/ConcaveHull_ACM_MYS.pdf
    _UTIL_ASSERT(false, "TODO finish concave hull implementation");
}

std::vector<std::size_t> convex_hull_for_3d_points_on_plane(const Vector3D& plane_normal,
                                                            const Vector3D& plane_point,
                                                            const std::vector<Vector3D>& points)
{
    // Compute the lower bound of points - plane_point (min x, min y, min z)
    Vector3D lower_bound = find_lower_bound(points);

    // We will start with the point that is fartherest from the plane point.  Since they are in the
    // same plane, then it should be the case that the farthest point is part of the hull.
    std::size_t starting_index = 0;
    double starting_distance   = std::numeric_limits<double>::min();
    for (auto i : utility::indices(points))
    {
        auto p      = points[i];
        double dist = (p - lower_bound).norm();
        if (dist > starting_distance)
        {
            starting_distance = dist;
            starting_index    = i;
        }
    }

    // Sort based on the angle
    auto reference_v = points[starting_index] - plane_point;
    auto point_index_sorted_by_angle =
      find_permutation_sorted_by_angle(points, plane_normal, plane_point, reference_v);
    assert(point_index_sorted_by_angle[0] == starting_index);

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

    // Note: special case
    while (convex_hull_point_indices.size() > 3)
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
        else
        {
            break;
        }
    }

    /*
    // Special cases
    //
    // Correction to the reference above, if the last point referenced by
    // point_index_sorted_by_angle happens to break convex hull, it will not be removed.  This
    // checks that the condition of the cross-product aligning with the normal holds for the last
    // point as well.  This is the first special case.
    //
    // Note: these should not apply anymore after the starting point was changed to the farthest
    // from the plane point. Another special case: the point closest to the lower bound might not
    // belong to the convex hull.  In 2D, this is true, but not always in 3D.  If this case occurs,
    // then there can be many other points that don't belong to the hull.
    std::vector<std::array<std::size_t, 3>> special_case_triplets = {
        { convex_hull_point_indices.size() - 2, convex_hull_point_indices.size() - 1, 0 } //,
        //{ convex_hull_point_indices.size() - 1, 0, 1 },
        //{ 0, 1, 2 }
    };
    for (auto case_i : utility::indices(special_case_triplets))
    {
        const auto& index_triplet = special_case_triplets[case_i];
        const auto& p1_index      = convex_hull_point_indices[index_triplet[0]];
        const auto& p2_index      = convex_hull_point_indices[index_triplet[1]];
        const auto& p3_index      = convex_hull_point_indices[index_triplet[2]];
        auto w = (points[p2_index] - points[p1_index]).cross(points[p3_index] - points[p1_index]);
        auto w_dot_normal = w.dot(plane_normal);
        if (w_dot_normal < 0)
        {
            convex_hull_point_indices.erase(convex_hull_point_indices.begin() + index_triplet[1]);
            // Adjust indices for indices when one is removed
            for (auto case_j :
                 utility::range((std::size_t)case_i + 1, special_case_triplets.size()))
            {
                for (auto& i : special_case_triplets[case_j])
                {
                    if (i >= index_triplet[1])
                    {
                        if (i == 0)
                        {
                            i = convex_hull_point_indices.size() - 1;
                        }
                        else
                        {
                            i--;
                        }
                    }
                }
            }
        }
    }*/

    return convex_hull_point_indices;
}

}