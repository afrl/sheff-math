#pragma once

#include "EigenDefines.h"

namespace simple_math
{
	/// Returns if a < b while taking into account a numerical tolerance
	bool compare_points(const Point& a, const Point& b, const double tolerance);
	
	/// Returns an object that acts like a Vector3D without copying the memory
	/// from the given point.  Provides the Eigen interface for a Point.
	Eigen::Map<const Vector3D> point_as_vector(const Point& p);
};