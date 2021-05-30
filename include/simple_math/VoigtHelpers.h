#pragma once

#include "Eigen/Eigenvalues"
#include "simple_math/EigenDefines.h"
#include "simple_math/Utility.h"
#include "utility/ContainerSearch.h"
#include "utility/Range.h"

namespace simple_math
{
/// @brief Converts a 3x3 matrix to the Voigt vector form.
/// 
/// Follows convention (xx, yy, zz, yz, xz, xy)
VoigtVector matrix_to_voigt(const Matrix3D& m);

/// @brief Converts a Voigt representation of a matrix back to its matrix form.
/// Note: This does not work properly for engineering strain due to the shear
///       terms convention.
Matrix3D voigt_to_matrix(const VoigtVector& v);

using EigenValueVectorPair = std::pair<std::complex<double>, Eigen::Vector3cd>;

/// @brief Finds the largest eigenvalue and corresponding eigenvector for the given 3x3 matrix.
/// @returns A pair of a complex value and a complex 3D vector
EigenValueVectorPair get_max_eigen_pair(const Matrix3D& m);
}