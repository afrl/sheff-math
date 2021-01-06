
#include "simple_math/Utility.h"

namespace simple_math
{
bool compare_points(const Point& a, const Point& b, const double tolerance)
{
    return (a[0] < b[0] - tolerance) ||
           ((fabs(a[0] - b[0]) < tolerance) && a[1] < b[1] - tolerance) ||
           ((fabs(a[0] - b[0]) < tolerance) && (fabs(a[1] - b[1]) < tolerance) &&
            (a[2] < b[2] - tolerance));
}

double distance(const Point& a, const Point& b)
{
    return sqrt((a[0] - b[0]) * (a[0] - b[0]) + (a[1] - b[1]) * (a[1] - b[1]) +
                (a[2] - b[2]) * (a[2] - b[2]));
}

Eigen::Map<const Vector3D> point_as_vector(const Point& p)
{
    return Eigen::Map<const Vector3D>(&p[0], p.size());
}

Vector3D point_to_vector(const Point& p)
{
    return Vector3D(p[0], p[1], p[2]);
}

std::vector<Point> linspace(const Point& p1, const Point& p2, const int& n_intervals)
{
    using namespace point_operators;

    std::vector<Point> result(n_intervals + 1, { 0, 0, 0 });
    for (int i = 0; i < n_intervals; ++i)
    {
        result[i] =
          p1 + ((double)i / (n_intervals)) * (p2 - p1); // This should avoid roundoff issues
    }
    // Make sure that the last value is exactly what you expect (no roundoff)
    result.back() = p2;
    return result;
}

std::vector<double> linspace(const double& start, const double& end, const int& n_intervals)
{
    std::vector<double> result(n_intervals + 1);
    for (int i = 0; i < n_intervals; ++i)
    {
        result[i] =
          start + ((double)i / (n_intervals)) * (end - start); // This should avoid roundoff issues
    }
    // Make sure that the last value is exactly what you expect (no roundoff)
    result.back() = end;
    return result;
}
std::string to_string(const simple_math::Point& point)
{
    return "(" + std::to_string(point[0]) + ", " + std::to_string(point[1]) + ", " +
           std::to_string(point[2]) + ")";
}
double calculate_mean(const std::vector<double>& data)
{
    double sum = 0.0;
    for (const auto& i : data)
    {
        sum += i;
    }
    const double mean = sum / (double)data.size();
    return mean;
}
double calculate_std_dev(const std::vector<double>& data)
{
    double mean    = calculate_mean(data);
    double std_dev = 0.0;
    for (auto i : data)
    {
        std_dev += pow(i - mean, 2);
    }
    std_dev = sqrt(std_dev / data.size());
    return std_dev;
}
};
