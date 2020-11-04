#include "..\include\simple_math\Intersection.h"

#include "simple_math/Intersection.h"

namespace simple_math
{
/// @brief Takes a guess of where an intersection should be and iteratively refines the guess closer
/// towards an intersection.
///
/// @return A bool indicating whether the intersection was found within tolerance and maximum #
/// iterations.
static bool refine_guess(const std::function<double(Point)>& signed_distance_function,
                         const Point& left_p,
                         const Point& right_p,
                         const double& segment_length,
                         double& guess_s,
                         Point& guess_p,
                         double& prev_s,
                         double& prev_residual,
                         const double segment_normalized_tol = 1e-6,
                         const int max_iterations            = 10)
{
    using namespace point_operators;

    const auto tol     = segment_normalized_tol * segment_length;
    int curr_iteration = 1;
    auto residual      = signed_distance_function(guess_p);
    double delta_s;
    while (abs(residual) > tol && curr_iteration <= max_iterations)
    {
        auto approx_grad = (residual - prev_residual) / (guess_s - prev_s);
        prev_s           = guess_s;
        delta_s          = -(residual / approx_grad); // Newton's method
        // Control Newton's method to make sure it does not greatly exceed the interior of the
        // segment
        if (delta_s > 0.5)
        {
            delta_s = 0.5;
        }
        else if (delta_s < 0.5)
        {
            delta_s = -0.5;
        }
        guess_s       = prev_s - (residual / approx_grad); // Newton's method
        prev_residual = residual;
        guess_p       = (1. - guess_s) * left_p + guess_s * right_p;
        residual      = signed_distance_function(guess_p);

        curr_iteration++;
    }

    return curr_iteration <= max_iterations;
}


std::vector<Intersection> find_segment_level_set_intersection(
  const std::function<double(Point)> signed_distance_function,
  const Point& p1,
  const double& p1_value,
  const Point& p2,
  const double& p2_value)
{
    using namespace point_operators;

    // Ensures determinant behavior regardless of the order of p1 and p2
    Point left_p, right_p;
    double left_value, right_value;
    if (p1 < p2)
    {
        left_p      = p1;
        left_value  = p1_value;
        right_p     = p2;
        right_value = p2_value;
    }
    else
    {
        left_p      = p2;
        left_value  = p2_value;
        right_p     = p1;
        right_value = p1_value;
    }

    const double segment_length         = distance(left_p, right_p);
    const double segment_normalized_tol = 1e-4;
    const auto tol                      = segment_normalized_tol * segment_length;
    auto left_sign                      = sign(left_value, tol);
    auto right_sign                     = sign(right_value, tol);

    std::vector<Intersection> intersections;

    if (left_sign == 0)
    {
        intersections.push_back({ left_p, Intersection::Type::ENDPOINT });
    }

    if (right_sign == 0)
    {
        intersections.push_back({ right_p, Intersection::Type::ENDPOINT });
    }

    if (left_sign != right_sign && left_sign != 0 && right_sign != 0)
    { // Guaranteed to intersect (if assumption holds that level-set is C-0)
        // s is the distance along the segment at which the intersections occurs.
        auto s                  = -left_value / (right_value - left_value);
        auto intersection_point = (1. - s) * left_p + s * right_p;

        // Use left_p as "previous guess" for finite difference approximation of gradient
        auto prev_s        = 0.0;
        auto prev_residual = left_value;
        bool success       = refine_guess(signed_distance_function,
                                    left_p,
                                    right_p,
                                    segment_length,
                                    s,
                                    intersection_point,
                                    prev_s,
                                    prev_residual);
        assert(success);

        intersections.push_back({ intersection_point, Intersection::Type::INTERIOR });
    }
    else
    {
        if (left_sign != 0 && abs(left_value) < segment_length)
        { // The segment could possibly intersect the level-set multiple times
            // Perform approximate gradient descent starting from each end point of the line
            // segment.
            const auto s_pertubation    = 0.1;
            auto left_search_s          = s_pertubation;
            auto left_search_p          = (1. - left_search_s) * left_p + left_search_s * right_p;
            auto prev_s                 = 0.0;
            auto prev_residual          = left_value;
            bool left_success           = refine_guess(signed_distance_function,
                                             left_p,
                                             right_p,
                                             segment_length,
                                             left_search_s,
                                             left_search_p,
                                             prev_s,
                                             prev_residual);
            const bool left_is_interior = left_success && left_search_s >= 0 && left_search_s <= 1;
            if (left_is_interior)
            {
                intersections.push_back({ left_search_p, Intersection::Type::INTERIOR });
            }
        }

        if (right_sign != 0 && abs(right_value) < segment_length)
        { // The segment could possibly intersect the level-set multiple times
            // Perform approximate gradient descent starting from each end point of the line
            // segment.
            const auto s_pertubation = 0.1;
            auto right_search_s      = 1.0 - s_pertubation;
            auto right_search_p      = (1. - right_search_s) * left_p + right_search_s * right_p;
            auto prev_s              = 1.0;
            auto prev_residual       = right_value;
            bool right_success       = refine_guess(signed_distance_function,
                                              left_p,
                                              right_p,
                                              segment_length,
                                              right_search_s,
                                              right_search_p,
                                              prev_s,
                                              prev_residual);
            bool right_is_interior   = right_success && right_search_s >= 0 && right_search_s <= 1;
            if (right_is_interior)
            {
                auto test = simple_math::distance(intersections[0].point, right_search_p);
                // If they are the same point, then assume it is a tangent intersection
                if (intersections.size() == 0 ||
                    simple_math::distance(intersections[0].point, right_search_p) > tol)
                { // There are two interior cuts
                    intersections.push_back({ right_search_p, Intersection::Type::INTERIOR });
                }
            }
        }
    }

    return intersections;
}

std::vector<Intersection> find_segment_level_set_intersection(
  const std::function<double(Point)> signed_distance_function,
  const Point& p1,
  const Point& p2)
{
    return find_segment_level_set_intersection(signed_distance_function,
                                               p1,
                                               signed_distance_function(p1),
                                               p2,
                                               signed_distance_function(p2));
}
}