#ifndef HUNGARIAN_H
#define HUNGARIAN_H

#include <vector>

struct WeightedBipartiteEdge {
    int left;
    int right;
    double cost;

    WeightedBipartiteEdge() : left(), right(), cost() {}
    WeightedBipartiteEdge(int left, int right, double cost)
      : left(left)
      , right(right)
      , cost(cost)
    {
    }
};

// Given the number of nodes on each side of the bipartite graph and a list of edges, returns a minimum-weight perfect matching.
// If a matching is found, returns a length-n vector, giving the nodes on the right that the left nodes are matched to.
// If no matching exists, returns an empty vector.
// (Note: Edges with endpoints out of the range [0, n) are ignored.)
std::vector<int> hungarianMinimumWeightPerfectMatching(const int n, const std::vector<WeightedBipartiteEdge> allEdges);


#endif // HUNGARIAN_H
