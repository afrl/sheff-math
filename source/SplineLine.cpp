#pragma once

#include "simple_math/SplineLine.h"

namespace simple_math
{
SplineLine3D::SplineLine3D(const std::vector<Point>& points,
                           const int s_0_index,
                           const int s_1_index)
  : points_ref_(points_as_matrix(points))
  , s_0_index_(s_0_index)
  , s_1_index_(s_1_index)
{
    update();
}

SplineLine3D::SplineLine3D(const std::vector<Point>& points)
  : points_ref_(points_as_matrix(points))
  , s_0_index_(0)
  , s_1_index_(points.size())
{
    update();
}


SplineLine3D::SplineLine3D(const DynamicDoubleMatrix& points)
  : points_ref_(points.data(), points.rows(), points.cols())
  , s_0_index_(0)
  , s_1_index_(points.rows())
{
    update();
}

SplineLine3D::Sample SplineLine3D::at_s(const double& s) const
{
    // Adjust s to consider s_0_index_ and s_1_index_
    const double adjusted_s = s * (s_(s_1_index_) - s_(s_0_index_)) + s_(s_0_index_);

    auto p = position_function_(adjusted_s);
    return { s, length_function_(adjusted_s)(0), { p(0), p(1), p(2) } };
}

void SplineLine3D::update()
{
    _UTIL_ASSERT(
      points_ref_.rows() < 10000,
      "Splines require an Ax=b solve, and this implementation uses a dense matrix with QR "
      "decomposition.  Therefore, the number of points are limited to 10000 without the needed "
      "for a lot of RAM.  A sparse system would be preferred and open up the possibility for "
      "more points, but is not implemented at this time.");

    d_.resize(points_ref_.rows(), 1);
    d_.setZero();
    total_length_ = 0.;
    for (auto i : utility::range(points_ref_.rows() - 1))
    {
        const double d = (points_ref_.row(i + 1) - points_ref_.row(i)).norm();
        total_length_ += d;
        d_(i + 1, 0) = d_(i, 0) + d;
    }

    // The coordinate s will correspond to the fractional distance along the line in the range from
    // 0 to 1 (not accounting for s_0_index_ and s_1_index_)
    s_ = d_ / total_length_;

    // Adjust the distances to that 0 falls at the index at which s = 0
    auto offset = d_(s_0_index_);
    for (auto i : utility::range(points_ref_.rows()))
    {
        d_(i) -= offset;
    }
    total_length_ = d_(s_1_index_);

    length_function_ =
      Eigen::SplineFitting<Spline1D>::Interpolate(d_.transpose(), 3, s_.transpose());

    // Transform the points to be x(s), y(s), z(s) where s is a parametric
    // coordinate in the range [0, 1].
    position_function_ =
      Eigen::SplineFitting<Spline3D>::Interpolate(points_ref_.transpose(), 3, s_.transpose());
}

}