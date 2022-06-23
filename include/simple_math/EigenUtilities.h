#pragma once

#include "EigenDefines.h"

namespace simple_math
{

/// @brief Map a raw array to a vector in the Eigen library.
template<typename T, typename SizeType = Eigen::Index>
Eigen::Map<DynamicVector<T>> * as_vector(T* const data, const SizeType size)
{
    return new Eigen::Map<DynamicVector<T>>(data, size);
}

/// @brief Map a raw array to a column-major matrix in the Eigen library.
template<typename T, typename SizeType = Eigen::Index>
Eigen::Map<DynamicMatrixColMajor<T>> * as_matrix(T* const data, const SizeType rows, const SizeType cols)
{
    return new Eigen::Map<DynamicMatrixColMajor<T>>(data, rows, cols);
}


}