#include "simple_math/Intersection.h"

#include <cmath>

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
                         double& guess_s,
                         Point& guess_p,
                         double& prev_s,
                         double& prev_residual,
                         const double tol         = 1e-10,
                         const int max_iterations = 20)
{
    using namespace point_operators;

    int curr_iteration = 1;
    auto residual      = signed_distance_function(guess_p);
    double delta_s;
    while (std::abs(residual) > tol && curr_iteration <= max_iterations)
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

static std::vector<double> guess_roots(const SignedDistanceFunction& signed_distance_function,
                                       const Point& left_p,
                                       const double& left_value,
                                       const Point& right_p,
                                       const double& right_value,
                                       const double valid_tol = 0.2)
{
    // Short-circuit the linear case
    if (signed_distance_function.approx_polynomial_order == 1)
    {
        const double root = -left_value / (right_value - left_value);
        if (root >= 0.0 && root <= 1.0)
        {
            return { -left_value / (right_value - left_value) };
        }
        else
        {
            return {};
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////
    // General procedure for fitting a polynomial of the approximate order

    using namespace point_operators;

    auto N = signed_distance_function.approx_polynomial_order + 1;
    auto s = linspace(0.0, 1.0, N - 1);
    simple_math::GeneralVector fs(N);
    fs(0)     = left_value;
    fs(N - 1) = right_value;
    for (auto i : utility::range<unsigned int>(1, N - 1))
    {
        Point p = (1.0 - s[i]) * left_p + s[i] * right_p;
        fs[i]   = signed_distance_function.f(p);
    }

    simple_math::GeneralMatrix s_matrix(N, N);
    for (auto i : utility::range(N))
    {
        for (auto j : utility::range(N))
        {
            s_matrix(i, j) = pow(s[i], j);
        }
    }

    simple_math::GeneralVector c = s_matrix.inverse() * fs;
    // c represents the coefficients that fits the level-set in the form:
    // fitted level-set = c[0] + c[1]*s + ... + c[p]*s^p, where
    // p = signed_distance_function.approx_polynomial_order

    ///////////////////////////////////////////////////////////////////////////////////
    // Find roots

    std::vector<double> roots;
    if (signed_distance_function.approx_polynomial_order == 2)
    { // y = c[0] + c[1]*x + c[2]*x^2
        const double d = c[1] * c[1] - 4 * c[2] * c[0];
        if (d < 0.0)
        {
            return {};
        }
        if (c[2] == 0.0)
        {
            const double root = -left_value / (right_value - left_value);
            if (root >= 0.0 && root <= 1.0)
            {
                return { -left_value / (right_value - left_value) };
            }
            else
            {
                return {};
            }
        }

        const double sd = sqrt(d);
        roots           = { (-c[1] - sd) / (2.0 * c[2]), (-c[1] + sd) / (2.0 * c[2]) };
    }
    // TODO implement cubic polynomial roots
    else
    {
        throw _UTIL_MSG_EXCEPT(
          "An analytical expression for roots is not implemented for polynomials of degree " +
          std::to_string(signed_distance_function.approx_polynomial_order));
    }

    std::vector<double> valid_roots;
    for (auto& r : roots)
    {
        if (r >= -valid_tol && r <= 1.0 + valid_tol)
        {
            valid_roots.push_back(r);
        }
    }
    return valid_roots;
}

std::vector<Intersection> find_segment_level_set_intersection(
  const SignedDistanceFunction& signed_distance_function,
  const Point& p1,
  const double& p1_value,
  const Point& p2,
  const double& p2_value,
  const double& tol)
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

    auto left_sign  = sign(left_value, tol);
    auto right_sign = sign(right_value, tol);

    std::vector<Intersection> intersections;

    if (left_sign == 0)
    {
        intersections.push_back({ left_p, Intersection::Type::ENDPOINT });
    }

    if (right_sign == 0)
    {
        intersections.push_back({ right_p, Intersection::Type::ENDPOINT });
    }

    auto root_guesses =
      guess_roots(signed_distance_function, left_p, left_value, right_p, right_value);
    for (auto& root_guess : root_guesses)
    {
        auto guess_s        = root_guess;
        auto guess_p        = (1. - guess_s) * left_p + guess_s * right_p;
        auto guess_residual = signed_distance_function.f(guess_p);

        auto perturbed_s        = guess_s + 0.05;
        auto perturbed_p        = (1. - perturbed_s) * left_p + perturbed_s * right_p;
        auto perturbed_residual = signed_distance_function.f(perturbed_p);

        bool success           = refine_guess(signed_distance_function.f,
                                    left_p,
                                    right_p,
                                    guess_s,
                                    guess_p,
                                    perturbed_s,
                                    perturbed_residual);
        const bool is_interior = success && guess_s >= 0 && guess_s <= 1;
        if (is_interior)
        {
            bool already_has_point = false;
            for (auto& intersection : intersections)
            {
                const double d = distance(guess_p, intersection.point);
                already_has_point |= d < tol;
            }
            if (!already_has_point)
            {
                intersections.push_back({ guess_p, Intersection::Type::INTERIOR });
            }
        }
    }

    return intersections;
}

std::vector<Intersection> find_segment_level_set_intersection(
  const SignedDistanceFunction& signed_distance_function,
  const Point& p1,
  const Point& p2)
{
    return find_segment_level_set_intersection(signed_distance_function,
                                               p1,
                                               signed_distance_function.f(p1),
                                               p2,
                                               signed_distance_function.f(p2));
}
}