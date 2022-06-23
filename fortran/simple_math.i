%module simple_math

//////////////////////////////////////////////////////////////////////////////////////////////////
// Import other SHEFF modules

%import utility.i

//////////////////////////////////////////////////////////////////////////////////////////////////
// simple_math bindings

%{
#include "simple_math/Defines.h"
#include "simple_math/EigenDefines.h"
#include "simple_math/EigenUtilities.h"
%}

%include "simple_math/Defines.h"

%include "simple_math/EigenDefines.h"
%template(MatrixDouble) simple_math::DynamicMatrixColMajorMap<double>;

%inline
%{
// A helper function to map a Fortran 1D array to an Eigen vector
static simple_math::DynamicVectorRef<double> as_vector(
  double* const data, const uint64_t size)
{
    return simple_math::DynamicVectorMap<double>(data, size);
}

// A helper function to map a Fortran 2D array to an Eigen matrix
static simple_math::DynamicMatrixColMajorRef<double> as_matrix(
  double* const data, const uint64_t rows, const uint64_t cols)
{
    // Account for the fact that Fortran uses col-major matrices, while C++ uses row-major.
    return simple_math::DynamicMatrixColMajorMap<double>(data, rows, cols);
}
%}