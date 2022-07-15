#pragma once

#include "BlockMatrix.h"
#include "BlockVector.h"
#include "Defines.h"
#include "HeapPreallocatedMatrix.h"
#include "utility/Exceptions.h"
#include "utility/Slice.h"
#include "utility/serialization/NamedValue.h"

#include <numeric>

namespace simple_math
{
using Quaternion       = Eigen::Quaternion<FloatType>;
using Vector2D         = Eigen::Matrix<FloatType, 2, 1>;
using Vector3D         = Eigen::Matrix<FloatType, 3, 1>;
using Matrix3D         = Eigen::Matrix<FloatType, 3, 3, Eigen::RowMajor>;
using Matrix2x3        = Eigen::Matrix<double, 2, 3, Eigen::RowMajor>;
using VoigtVector      = Eigen::Matrix<FloatType, 6, 1>;
using VoigtMatrix      = Eigen::Matrix<FloatType, 6, 6, Eigen::RowMajor>;
using GeneralVector    = Eigen::Matrix<FloatType, Eigen::Dynamic, 1>;
using GeneralMatrix    = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using EigenBlockVector = BlockVector<GeneralVector>;
using EigenBlockMatrix = BlockMatrix<GeneralMatrix>;

template<typename T>
using DynamicVector = Eigen::Matrix<T, Eigen::Dynamic, 1>;
template<typename T>
using DynamicMatrix = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
template<typename T>
using DynamicMatrixColMajor = Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic, Eigen::ColMajor>;

using DynamicDoubleVector = DynamicVector<double>;
using DynamicInt16Vector  = DynamicVector<int16_t>;
using DynamicInt32Vector  = DynamicVector<int32_t>;
using DynamicInt64Vector  = DynamicVector<int64_t>;
using DynamicUInt16Vector = DynamicVector<uint16_t>;
using DynamicUInt32Vector = DynamicVector<uint32_t>;
using DynamicUInt64Vector = DynamicVector<uint64_t>;
using DynamicBoolVector   = DynamicVector<bool>;

using DynamicDoubleMatrix = DynamicMatrix<double>;
using DynamicInt8Matrix   = DynamicMatrix<int8_t>;
using DynamicInt16Matrix  = DynamicMatrix<int16_t>;
using DynamicInt32Matrix  = DynamicMatrix<int32_t>;
using DynamicInt64Matrix  = DynamicMatrix<int64_t>;
using DynamicUInt8Matrix  = DynamicMatrix<uint8_t>;
using DynamicUInt16Matrix = DynamicMatrix<uint16_t>;
using DynamicUInt32Matrix = DynamicMatrix<uint32_t>;
using DynamicUInt64Matrix = DynamicMatrix<uint64_t>;
using DynamicBoolMatrix   = DynamicMatrix<bool>;

// Useful aliases that map raw arrays to Eigen objects.
template<typename T>
using DynamicVectorMap = Eigen::Map<DynamicVector<T>>;
template<typename T>
using DynamicMatrixMap = Eigen::Map<DynamicMatrix<T>>;
template<typename T>
using DynamicMatrixColMajorMap = Eigen::Map<DynamicMatrixColMajor<T>>;

// Useful aliases to treat references to Eigen objects.
template<typename T>
using DynamicVectorRef = Eigen::Ref<DynamicVector<T>>;
template<typename T>
using DynamicMatrixRef = Eigen::Ref<DynamicMatrix<T>>;
template<typename T>
using DynamicMatrixColMajorRef = Eigen::Ref<DynamicMatrixColMajor<T>>;
}

// Provide serialization specialization for Eigen types
namespace utility
{
/// Eigen::Quaternion<FloatType> serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const Eigen::Quaternion<simple_math::FloatType>& quaternion)
{
    s(named_value("w", quaternion.w()),
      named_value("x", quaternion.x()),
      named_value("y", quaternion.y()),
      named_value("z", quaternion.z()));
}

/// Eigen::Quaternion<FloatType> deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, Eigen::Quaternion<simple_math::FloatType>& quaternion)
{
    s(quaternion.w(), quaternion.x(), quaternion.y(), quaternion.z());
}


/// simple_math::Vector3D serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const simple_math::Vector3D& v)
{
    s(named_value("data", slice(v.data(), v.rows())));
}

/// simple_math::Vector3D deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, simple_math::Vector3D& v)
{
    s(slice(v.data(), v.rows()));
}


/// simple_math::VoigtVector serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const simple_math::VoigtVector& v)
{
    s(named_value("size", v.rows()));
    s(named_value("data", slice(v.data(), v.rows())));
}

/// simple_math::VoigtVector deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, simple_math::VoigtVector& v)
{
    Eigen::Index v_size;
    s(v_size);
    v.resize(v_size);
    s(slice(v.data(), v.rows()));
}


/// simple_math::GeneralVector serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const simple_math::GeneralVector& v)
{
    s(named_value("size", v.rows()));
    s(named_value("data", slice(v.data(), v.rows())));
}

/// simple_math::GeneralVector deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, simple_math::GeneralVector& v)
{
    Eigen::Index v_size;
    s(v_size);
    v.resize(v_size);
    s(slice(v.data(), v.rows()));
}

namespace detail
{
/// simple_math::GeneralVector serialize(...) specialization
template<typename SerializerType, typename MatrixType>
void serialize_matrix(SerializerType& s, const MatrixType& m)
{
    s(utility::named_value("rows", m.rows()));
    s(utility::named_value("cols", m.cols()));
    s(utility::named_value("data", utility::slice(m.data(), m.rows() * m.cols())));
}

/// simple_math::GeneralVector deserialize(...) specialization
template<typename SerializerType, typename MatrixType>
void deserialize_matrix(SerializerType& s, MatrixType& m)
{
    Eigen::Index n_rows;
    s(n_rows);

    Eigen::Index n_cols;
    s(n_cols);

    m.resize(n_rows, n_cols);
    s(utility::slice(m.data(), m.rows() * m.cols()));
}
}

/// simple_math::DynamicDoubleMatrix serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const simple_math::DynamicDoubleMatrix& m)
{
    detail::serialize_matrix(m);
}

/// simple_math::DynamicDoubleMatrix deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, simple_math::DynamicDoubleMatrix& m)
{
    detail::deserialize_matrix(m);
}

/// simple_math::Matrix2x3 serialize(...) specialization
template<typename SerializerType>
void serialize(SerializerType& s, const simple_math::Matrix2x3& m)
{
    detail::serialize_matrix(s, m);
}

/// simple_math::Matrix2x3 deserialize(...) specialization
template<typename SerializerType>
void deserialize(SerializerType& s, simple_math::Matrix2x3& m)
{
    detail::deserialize_matrix(s, m);
}
}

// Eigen::Quaternion<FloatType>

// Note: after benchmarking, it appears that memory allocation is neglible compared
// to the time required to compute quantities in the workspace (as I should have anticipated).
// Consequently, it is not useful to specialize fixed size arrays.  It provides less than
// 5% faster performance, but at the cost of code clarity and fighting stack allocation
// limits.  So, it does not appear worthwhile.

/* depreciated

// Constants that specify the limits of various quantities within an element
// that in turn are used to create fixed Eigen matrix types
static constexpr int MAX_N_NODES = 27;
static constexpr int MAX_N_DOFS_PER_NODE = 15;
static constexpr int MAX_N_QUAD_POINTS = 27;
static constexpr int MAX_N_NODE_DOFS_PER_ELEMENT = MAX_N_NODES * MAX_N_DOFS_PER_NODE;
static constexpr int MAX_N_QUAD_DOFS_PER_ELEMENT = MAX_N_QUAD_POINTS * MAX_N_DOFS_PER_NODE;

// Containers for matrices involving the number of DoF's in an element points
using ElementDofVector = Eigen::Matrix<FloatType, Eigen::Dynamic, 1,
    Eigen::ColMajor, MAX_N_NODE_DOFS_PER_ELEMENT>;
using ElementDofX3Matrix = HeapPreallocatedMatrix < FloatType,
    MAX_N_NODE_DOFS_PER_ELEMENT, 3>;
using ElementDofX6Matrix = HeapPreallocatedMatrix < FloatType,
    MAX_N_NODE_DOFS_PER_ELEMENT, 6>;
using Element6XDofMatrix = HeapPreallocatedMatrix < FloatType,
    6, MAX_N_NODE_DOFS_PER_ELEMENT>;
using ElementDofXDofMatrix = HeapPreallocatedMatrix<FloatType,
    MAX_N_NODE_DOFS_PER_ELEMENT, MAX_N_NODE_DOFS_PER_ELEMENT>;

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

*/
