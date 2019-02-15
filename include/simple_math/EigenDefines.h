#pragma once

#define EIGEN_MAX_CPP_VER 11
//#define EIGEN_STACK_ALLOCATION_LIMIT 460800
#include "Eigen/Eigen"

#include "Defines.h"

#include <numeric>

namespace simple_math
{
	// Constants that specify the limits of various quantities within an element
	// that in turn are used to create fixed Eigen matrix types
	static constexpr int MAX_N_NODES = 20;
	static constexpr int MAX_N_DOFS_PER_NODE = 4;
	static constexpr int MAX_N_QUAD_POINTS = 27;
	static constexpr int MAX_N_NODE_DOFS_PER_ELEMENT = MAX_N_NODES * MAX_N_DOFS_PER_NODE;
	static constexpr int MAX_N_QUAD_DOFS_PER_ELEMENT = MAX_N_QUAD_POINTS * MAX_N_DOFS_PER_NODE;

	using Quaternion = Eigen::Quaternion<FloatType>;
	using Vector3D = Eigen::Matrix<FloatType, 3, 1>;
	using Matrix3D = Eigen::Matrix<FloatType, 3, 3>;
	using VoigtVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1, 0, 6>;
	using VoigtMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, 0, 6, 6>;
	using GeneralVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1>;
	using GeneralMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic>;

	// Containers for matrices involing the max # of DoF's per an element (used for element
	// matrices/vectors that will be assembled into the global system)
	/// A vector with a fixed size that is larger than the connectivity expected for any element.
	using ElementDofVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1,
		Eigen::ColMajor, MAX_N_NODE_DOFS_PER_ELEMENT>;
	/// A matrix with a fixed size that is larger than the connectivity expected for any element
	/// and 3 columns for spatial derivatives.
	using ElementDofX3Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, MAX_N_NODE_DOFS_PER_ELEMENT, 3>;
	using ElementDofX6Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, MAX_N_NODE_DOFS_PER_ELEMENT, 6>;
	using Element6XDofMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, 6, MAX_N_NODE_DOFS_PER_ELEMENT>;
	/// A matrix with both dimensions larger than the amount of DoF's expected for a single element
	using ElementDofXDofMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, 96, 96>;

	// Containers for matrices involving the number of quad points
	using QuadVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1,
		Eigen::ColMajor, MAX_N_QUAD_POINTS>;
	using QuadX3Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 3,
		Eigen::ColMajor, MAX_N_QUAD_POINTS>;
	using QuadX4Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 4,
		Eigen::ColMajor, MAX_N_QUAD_POINTS>;
	using QuadX6Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 6,
		Eigen::ColMajor, MAX_N_QUAD_POINTS>;
	using QuadXNodeMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, MAX_N_QUAD_POINTS, MAX_N_NODES>;
	using QuadXDofMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, MAX_N_QUAD_POINTS, MAX_N_DOFS_PER_NODE>;
	using QuadDofX3Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 3,
		Eigen::ColMajor, MAX_N_QUAD_DOFS_PER_ELEMENT>;
	using VoigtXQuadMatrix = Eigen::Matrix<FloatType, 6, Eigen::Dynamic,
		Eigen::ColMajor, 6, MAX_N_QUAD_POINTS>;
	using QuadDofVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1,
		Eigen::ColMajor, MAX_N_QUAD_DOFS_PER_ELEMENT>;

	// Containers for matrices involving the number of nodes in the element
	using NodeVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1,
		Eigen::ColMajor, MAX_N_NODES>;
	using NodeX3Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 3,
		Eigen::ColMajor, MAX_N_NODES>;
	using NodeX6Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 6,
		Eigen::ColMajor, MAX_N_NODES>;
	using NodeXDofMatrix = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic,
		Eigen::ColMajor, MAX_N_NODES, MAX_N_DOFS_PER_NODE>;

	using DofX3Matrix = Eigen::Matrix<FloatType, Eigen::Dynamic, 3,
		Eigen::ColMajor, MAX_N_DOFS_PER_NODE>;
};