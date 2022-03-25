#pragma once

#include "simple_math/EigenDefines.h"
#include "simple_math/Utility.h"

#include <unsupported/Eigen/Splines>

namespace simple_math
{

class SplineLine3D
{
  public:
    struct Sample
    {
        double parametric_coordinate;
        double distance_along_line;
        Point point;
    };

    SplineLine3D(const std::vector<Point>& points, const int s_0_index, const int s_1_index);
    SplineLine3D(const std::vector<Point>& points);
    SplineLine3D(const DynamicDoubleMatrix& points);

    /// Samples the line at a given s
    Sample at_s(const double& s) const;

    /// Updates the internal spline functions
    /// Call this if the structure of points is the same, but the values have changed.
    void update();

  private:
    using Spline1D = Eigen::Spline<double /*scalar*/, 1 /*dim*/, 3 /*degree*/>;
    using Spline3D = Eigen::Spline<double /*scalar*/, 3 /*dim*/, 3 /*degree*/>;

    /// A reference to the points given to the constructor
    Eigen::Map<const DynamicDoubleMatrix> points_ref_;
    /// Sometimes extra points are included at the beginning and end of the list ensure a smooth
    /// spline near the end.  This is the index that lies at s = 0 (the start of the interval of the
    /// real range).
    Eigen::Index s_0_index_;
    /// Sometimes extra points are included at the beginning and end of the list ensure a smooth
    /// spline near the end.  This is the index that lies at s = 1 (the end of the interval of the
    /// real range).
    Eigen::Index s_1_index_;
    /// Distance along the line for each point given to the constructor
    simple_math::DynamicDoubleMatrix d_;
    /// Parametric coordinate in range [0, 1] of each point given to the constructor
    simple_math::DynamicDoubleMatrix s_;
    /// A spline fit of the distance along the line as a function of s, d(s)
    Spline1D length_function_;
    /// Coefficients for the spline that captures { x(s), y(s), z(s) }
    Spline3D position_function_;
    /// The length of the line in the global coordinate system (not parametric)
    double total_length_;
};

}