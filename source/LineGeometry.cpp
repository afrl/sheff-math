#include "simple_math/LineGeometry.h"

#include "simple_math/Utility.h"
#include "utility/Permutation.h"

namespace simple_math
{

using namespace point_operators;

SegmentClosestPoint closest_point_on_line_segment(const Point& p,
                                                  const Point& segment_p1,
                                                  const Point& segment_p2)
{
    // This implementation is loosely based on:
    // https://codereview.stackexchange.com/questions/162447/shortest-distance-from-point-to-line-segment-in-3d

    const auto p_vec      = point_as_vector(p);
    const auto p1_vec     = point_as_vector(segment_p1);
    const auto p2_vec     = point_as_vector(segment_p2);
    auto p1_p             = p_vec - p1_vec;
    auto p1_p2            = p2_vec - p1_vec;
    auto line_unit_vector = p1_p2.normalized();

    // Projection of p onto line p1->p2
    auto s = p1_p.dot(line_unit_vector);
    if (s <= 0.)
    {
        return { SegmentClosestPoint::END_POINT_1, distance(p, segment_p1), segment_p1 };
    }
    else if (s >= 1.)
    {
        return { SegmentClosestPoint::END_POINT_2, distance(p, segment_p2), segment_p2 };
    }
    else
    {
        auto closest_point = to_point(p1_vec + s * line_unit_vector);
        return { SegmentClosestPoint::INTERIOR, distance(p, closest_point), closest_point };
    }
}

LineClosestPoint2D closest_point_on_line_2D(const Vector2D& p,
                                            const Vector2D& segment_p1,
                                            const Vector2D& segment_p2)
{
    auto p1_p             = p - segment_p1;
    auto line_unit_vector = (segment_p2 - segment_p1).normalized();

    // Projection of p onto line p1->p2
    auto s             = p1_p.dot(line_unit_vector);
    Vector2D closest_point = segment_p1 + s * line_unit_vector;
    return { (p - closest_point).norm(), closest_point, s, line_unit_vector };
}

CircleIntersection intersect_circle_line(const double& r,
                                         const Point& center,
                                         const Point& line_p1,
                                         const Point& line_p2)
{
    const double length       = distance(line_p1, line_p2);
    const auto p1             = line_p1 - center;
    const auto p2             = line_p2 - center;
    const double D            = p1[0] * p2[1] - p2[0] * p1[1];
    const double discriminant = r * r * length * length - D * D;
    if (discriminant < 0.0)
    {
        // No intersection
        return CircleIntersection();
    }
    else if (discriminant == 0.0)
    {
        auto s                = p2 - p1;
        const double s_y_sign = s[1] < 0.0 ? -1.0 : 1.0;
        auto intersect_x      = D * s[1] / (length * length) + center[0];
        auto intersect_y      = -D * s[0] / (length * length) + center[1];
        return { CircleIntersection::TANGENT, { intersect_x, intersect_y, 0.0 } };
    }
    else
    {
        const auto s          = p2 - p1;
        const double s_y_sign = s[1] < 0.0 ? -1.0 : 1.0;
        const auto sqrt_disc  = sqrt(discriminant);
        const auto l_sq       = length * length;

        auto intersect_1_x = (D * s[1] + s_y_sign * s[0] * sqrt_disc) / l_sq + center[0];
        auto intersect_1_y = (-D * s[0] + abs(s[1]) * sqrt_disc) / l_sq + center[1];
        auto intersect_2_x = (D * s[1] - s_y_sign * s[0] * sqrt_disc) / l_sq + center[0];
        auto intersect_2_y = (-D * s[0] - abs(s[1]) * sqrt_disc) / l_sq + center[1];

        const Point intersect_1 = { intersect_1_x, intersect_1_y, 0.0 };
        const Point intersect_2 = { intersect_2_x, intersect_2_y, 0.0 };

        // We do want them sorted such that intersect_1 < intersect_2 when projected on the line
        const auto unit_segment =
          (point_as_vector(line_p2) - point_as_vector(line_p1)).normalized();
        const Vector3D v_1 = (point_as_vector(intersect_1) - point_as_vector(line_p1)).normalized();
        const double r_1   = v_1.dot(unit_segment);
        const Vector3D v_2 = (point_as_vector(intersect_1) - point_as_vector(line_p1)).normalized();
        const double r_2   = v_1.dot(unit_segment);

        if (r_1 < r_2)
        {
            return { CircleIntersection::TWO_INTERSECTS, intersect_1, intersect_2 };
        }
        else
        {
            return { CircleIntersection::TWO_INTERSECTS, intersect_2, intersect_1 };
        }
    }
}

CircleIntersection intersect_circle_line_segment(const double& r,
                                                 const Point& center,
                                                 const Point& segment_p1,
                                                 const Point& segment_p2)
{
    auto intersect = intersect_circle_line(r, center, segment_p1, segment_p2);

    const auto p1_vec       = point_as_vector(segment_p1);
    const auto p2_vec       = point_as_vector(segment_p2);
    const auto unit_segment = (p2_vec - p1_vec).normalized();
    const auto l            = distance(segment_p1, segment_p2);

    // Evaluate where the intersections fall on the line segment
    const auto seg_length       = distance(segment_p1, segment_p2);
    bool intersect_1_is_outside = false;
    if (intersect.type == CircleIntersection::TANGENT ||
        intersect.type == CircleIntersection::TWO_INTERSECTS)
    {
        const Vector3D v_1     = (point_as_vector(intersect.intersect_1) - p1_vec) / l;
        const double r         = v_1.dot(unit_segment);
        intersect_1_is_outside = r < 0.0 || r > 1.0;
    }
    bool intersect_2_is_outside = false;
    if (intersect.type == CircleIntersection::TWO_INTERSECTS)
    {
        const Vector3D v_2     = (point_as_vector(intersect.intersect_2) - p1_vec) / l;
        const double r         = v_2.dot(unit_segment);
        intersect_2_is_outside = r < 0.0 || r > 1.0;
    }

    // Modify the intersection data to account for the fact that it is a line segment instead of
    // infinite line.
    if (intersect.type == CircleIntersection::TWO_INTERSECTS)
    {
        if (intersect_1_is_outside && intersect_2_is_outside)
        {
            intersect.intersect_1 = { 0., 0., 0. };
            intersect.intersect_2 = { 0., 0., 0. };
            intersect.type        = CircleIntersection::NO_INTERSECTION;
        }
        else if (intersect_1_is_outside)
        {
            intersect.intersect_1 = intersect.intersect_2;
            intersect.intersect_2 = { 0., 0., 0. };
            intersect.type        = CircleIntersection::ONE_INTERSECT;
        }
        else if (intersect_2_is_outside)
        {
            intersect.intersect_2 = { 0., 0., 0. };
            intersect.type        = CircleIntersection::ONE_INTERSECT;
        }
    }
    else if (intersect.type == CircleIntersection::TANGENT)
    {
        if (intersect_1_is_outside)
        {
            intersect.intersect_1 = { 0., 0., 0. };
            intersect.type        = CircleIntersection::NO_INTERSECTION;
        }
    }

    return intersect;
}

bool is_point_in_polygon(const Point& p, const std::vector<Point>& polygon_points)
{
    bool is_inside = false;
    for (std::size_t i = 0, j = polygon_points.size() - 1; i < polygon_points.size(); j = i++)
    {
        const bool y_is_between_segment_points =
          ((polygon_points[i][1] <= p[1]) && (p[1] < polygon_points[j][1])) ||
          ((polygon_points[j][1] <= p[1]) && (p[1] < polygon_points[i][1]));
        if (y_is_between_segment_points &&
            (p[0] <= (polygon_points[j][0] - polygon_points[i][0]) * (p[1] - polygon_points[i][1]) /
                         (polygon_points[j][1] - polygon_points[i][1]) +
                       polygon_points[i][0]))
        {
            is_inside = !is_inside;
        }
        if (p[0] == polygon_points[i][0] && p[1] == polygon_points[i][1])
        {
            return true;
        }
    }
    return is_inside;
}

std::vector<std::size_t> find_sort_for_points_on_line(const Point& line_p1,
                                                      const Point& line_p2,
                                                      const std::vector<Point>& points)
{
    const auto p1_vec    = point_as_vector(line_p1);
    const auto p2_vec    = point_as_vector(line_p2);
    const auto unit_line = (p2_vec - p1_vec).normalized();
    const auto l         = distance(line_p2, line_p1);

    std::vector<double> points_parametric_distances;
    for (auto& p : points)
    {
        const Vector3D v = (point_as_vector(p) - p1_vec) / l;
        const double r   = v.dot(unit_line);
        points_parametric_distances.push_back(r);
    }
    const auto permuation = utility::find_sorted_permutation(points_parametric_distances);
    return permuation;
}

}