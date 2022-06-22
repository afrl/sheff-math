#include "simple_math/HungarianAlgorithm.h"

#include "hungarian_algorithm_detail/hungarian.h"
#include "simple_math/Utility.h"
#include "utility/Exceptions.h"
#include "utility/Range.h"

namespace simple_math
{
std::vector<int> solve_linear_sum_assignment(const std::vector<Point>& point_set_1,
                                             const std::vector<Point>& point_set_2)
{
    _UTIL_ASSERT(point_set_1.size() == point_set_2.size(),
                 "The two sets of points must be the same length.");
    _UTIL_ASSERT(
      point_set_1.size() < std::numeric_limits<int>::max(),
      "The number of points must be less than the maximum value for int to avoid overflow.");

    // First, we create a list of all possible edges. This should be the full list of edges
    // connecting set 1 to set 2 (N^2 edges).
    std::vector<WeightedBipartiteEdge> edges;
    edges.reserve(point_set_1.size() * point_set_1.size());
    for (auto i : utility::indices(point_set_1))
    {
        for (auto j : utility::indices(point_set_1))
        {
            edges.push_back({ static_cast<int>(i),
                              static_cast<int>(j),
                              simple_math::distance(point_set_1[i], point_set_2[j]) });
        }
    }

    return hungarianMinimumWeightPerfectMatching(static_cast<int>(point_set_1.size()), edges);
}
}