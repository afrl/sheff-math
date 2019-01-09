#pragma once

#include "simple_math/Utility.h"

namespace simple_math
{
	bool compare_points(const Point & a, const Point & b, const double tolerance)
	{
		return (a[0] < b[0] - tolerance)
			|| ((fabs(a[0] - b[0]) < tolerance) && a[1] < b[1] - tolerance)
			|| ((fabs(a[0] - b[0]) < tolerance) && (fabs(a[1] - b[1]) < tolerance) && (a[2] < b[2] - tolerance));
	}

	Eigen::Map<const Vector3D> point_as_vector(const Point & p)
	{
		return Eigen::Map<const Vector3D>(&p[0], p.size());
	}
};