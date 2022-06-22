#pragma once

#include "simple_math/Defines.h"

#include <vector>

namespace simple_math
{
/// @brief Finds the 1-to-1 connections between two sets of points with the minimum total distance.
///
/// Uses an O(n^3) implementation of the Hungarian algorithm to solve the one-to-one assignment
/// problem. In graph theory, this is equivalent to finding the weighted bi-partite perfect
/// matching subgraph. Refer to https://en.wikipedia.org/wiki/Hungarian_algorithm for
/// a nice description of the problem.
///
/// @param point_set_1 - a set of 3D points with length N
/// @param point_set_2 - a second set of 3D points with length N
/// @returns A vector indicating the index map for connections from set 1 to set 2. For example,
///     the first entry indicates which point in the second set that the first point in set 1
///     should be connected to.
///
/// Implementation: https://github.com/jamespayor/weighted-bipartite-perfect-matching
std::vector<int> solve_linear_sum_assignment(const std::vector<Point>& point_set_1,
                                             const std::vector<Point>& point_set_2);
}