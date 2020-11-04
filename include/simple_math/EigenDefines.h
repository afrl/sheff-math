#pragma once

#include "BlockMatrix.h"
#include "BlockVector.h"
#include "Defines.h"
#include "HeapPreallocatedMatrix.h"
#include "utility/Exceptions.h"

#include <numeric>

namespace simple_math
{
using Quaternion       = Eigen::Quaternion<FloatType>;
using Vector3D         = Eigen::Matrix<FloatType, 3, 1>;
using Matrix3D         = Eigen::Matrix<FloatType, 3, 3>;
using VoigtVector      = Eigen::Matrix<FloatType, Eigen::Dynamic, 1, 0, 6>;
using VoigtMatrix      = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, 0, 6, 6>;
using GeneralVector    = Eigen::Matrix<FloatType, Eigen::Dynamic, 1>;
using GeneralMatrix    = Eigen::Matrix<FloatType, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
using EigenBlockVector = BlockVector<GeneralVector>;
using EigenBlockMatrix = BlockMatrix<GeneralMatrix>;
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