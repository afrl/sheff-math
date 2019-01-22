#pragma once

#define EIGEN_MAX_CPP_VER 11
#include "Eigen/Eigen"

#include <numeric>
#include <array>

namespace simple_math
{
	using FloatType = double;
	using Point = std::array<FloatType, 3>;
	using Quaternion = Eigen::Quaternion<FloatType>;
	using Vector3D = Eigen::Matrix<FloatType, 3, 1>;
	using Matrix3D = Eigen::Matrix<FloatType, 3, 3>;
	using VoigtVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1, 0, 6>;
	using VoigtMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, 0, 6, 6>;
	/// A vector with a fixed size that is larger than the connectivity expected for any element.
	using ElementVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1, 0, 120>;
	/// A matrix with a fixed size that is larger than the connectivity expected for any element
	/// and 3 columns for spatial derivatives.
	using ElementMatrix3D = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, 0, 120, 3>;
	/// A matrix with both dimensions larger than the amount of DoF's expected for a single element
	using ElementMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, 120, 120>;
};