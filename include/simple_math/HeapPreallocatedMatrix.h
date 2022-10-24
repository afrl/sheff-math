#pragma once

#include "Eigen/Eigen"
#include "utility/Exceptions.h"

namespace simple_math
{
/// A fixed size array that is allocated on the heap instead of the stack, thus
/// avoiding stack allocation limits.  This class was considered for storing the
/// stiffness matrix in the element workspace (since the stack allocation limit
/// would prohibit the use of a fixed size array).  However, after benchmarking,
/// it was shown that though the resizing and manipulation of this heap allocated
/// fixed array was 50% faster than dynamically allocating a matrix, the assignment
/// operation that is necessary for a library to make a copy of it caused the overall
/// cost to be slower than just passing a dynamically created matrix via move semantics.
/// However, there are a few considerations for furture cases:
/// 1) If the matrix is used more than once during a calculation block, then it is better
///    preallocate via this class and store it in the workspace.
/// 2) If the matrix is only used once and requires storage, then it is better to dynamically
///    create the matrix and pass via move semantics.
/// 3) If the matrix is used once (or more) and requires assembly term-by-term, then it is
///    better to preallocate via this class.
/// 4) If the matrix is small enough to fit on the stack, just use a normal Eigen matrix
///    with the max rows and cols specified.
/// Though this class may not be used now, it may prove useful in the future.
template<typename _Scalar, int _MaxRows, int _MaxCols, int _Options = Eigen::ColMajor>
class HeapPreallocatedMatrix
  : public Eigen::Map<Eigen::Matrix<_Scalar, Eigen::Dynamic, Eigen::Dynamic, _Options>>
{
  public:
    HeapPreallocatedMatrix()
      : Eigen::Map<Eigen::Matrix<_Scalar, Eigen::Dynamic, Eigen::Dynamic, _Options>>(
          new _Scalar[_MaxRows * _MaxCols],
          _MaxRows,
          _MaxCols)
    {
        underlying_array_ = this->data();
    }

    ~HeapPreallocatedMatrix()
    {
        delete[] underlying_array_;
    }

    /// Resizes the apparent number of rows and columns without a memory rellocation.
    void resize(int rows, int cols)
    {
        _UTIL_ASSERT_C(rows * cols <= _MaxRows * _MaxCols,
                       "The number of rows and columns would exceed the preallocated space.");

        // Placement new operator will construct a new HeapPreallocatedMatrix object on the same
        // location of "this" replacing the current object.  This is pretty tricky, but it avoids a
        // new allocation while maintaining inheritance from Eigen::Map (allowing the object to
        // behave just like an Eigen::Matrix).
        new (this)
          HeapPreallocatedMatrix<_Scalar, _MaxRows, _MaxCols, _Options>(&underlying_array_[0],
                                                                        rows,
                                                                        cols);
    }

    // Currently, the = operator will just assign the value of the pointer.  Of course, a memcopy is
    // actually needed (as long as the other object is not "this"), so this needs to be implemented
    // if this class is ever used.
    void operator=(const HeapPreallocatedMatrix&) = delete;

  private:
    _Scalar* underlying_array_;

    /// A private constructor to help with resizing via placement new operator
    HeapPreallocatedMatrix(_Scalar* underlying_array, int rows, int cols)
      : underlying_array_(underlying_array)
      , Eigen::Map<Eigen::Matrix<_Scalar, Eigen::Dynamic, Eigen::Dynamic, _Options>>(
          &underlying_array[0],
          rows,
          cols)
    {
    }
};
};