#include "simple_math/PlaneGeometry.h"
#include "simple_math/Utility.h"

#include <assert.h>

using namespace simple_math;

int main()
{
    /////////////////////////////////////////////////////////////////////////////////////////////
    // Test plane_2d_projection and reverse_2d_project

    Vector3D plane_normal = DynamicDoubleVector::Random(3);
    plane_normal.normalize();
    Vector3D plane_point  = DynamicDoubleVector::Random(3);
    std::vector<Vector3D> rand_points(10);
    for (auto& p : rand_points)
    {
        p = DynamicDoubleVector::Random(3);
    }

    auto result = plane_2d_projection(plane_normal, plane_point, rand_points);

    auto rand_points_2 = reverse_2d_project(plane_normal, plane_point, result.points_2d, result.w);

    for (auto i : utility::indices(rand_points_2))
    {
        auto& ref  = rand_points[i];
        auto& comp = rand_points_2[i];
        auto diff  = (rand_points[i] - rand_points_2[i]).norm();
        assert((rand_points[i] - rand_points_2[i]).norm() < 1e-8);
    }

    return 0;
}