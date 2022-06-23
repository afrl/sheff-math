%module simple_math


//////////////////////////////////////////////////////////////////////////////////////////////////
// Import std typemaps

%include <typemaps.i>
%include <std_array.i>
%include <stdint.i>

//////////////////////////////////////////////////////////////////////////////////////////////////
// Import numpy typemaps

%{
#define SWIG_FILE_WITH_INIT
%}

%include "numpy.i"

%init
%{
    import_array();
%}

//////////////////////////////////////////////////////////////////////////////////////////////////
// Import other SHEFF modules

%import utility.i

//////////////////////////////////////////////////////////////////////////////////////////////////
// simple_math bindings

// Eigen::Ref presents a uniquely difficult type to wrap for a few reasons:
// 1. It has not default constructor
// 2. Wrapping the full Eigen class hierarchy is not feasible
// 3. It is templated
// 4. Eigen::Ref& makes no sense (refer to https://stackoverflow.com/questions/21132538/correct-usage-of-the-eigenref-class)
//
// Really, we just need a way to create an Eigen::Ref that refers to the underlying data in an
// numpy array. This will allow numpy arrays to be passed into C++ methods that take an
// Eigen::Ref. Of couse, we have to handle three types of references:
// 1. References to vectors
// 2. References to row-major arrays
// 3. References to col-major arrays (which may not be actually used)
//
// To accomplish this, we will tell Swig what methods of Eigen::Ref, which will just be the destructor.
// Importantly, the code outside %{ %} are not C++, it just tells SWIG what to do.  So we will
// Just define a structure for the different Eigen::Ref's we need (we will use the typedef'ed names
// for simplicity). Importantly, we will indicate to SWIG that there is no default constructor.
namespace simple_math
{
template<typename T>
class DynamicVectorRef
{
public:
DynamicVectorRef(const DynamicVectorRef&);
virtual ~DynamicVectorRef() {}
};

template<typename T>
class DynamicMatrixRef
{
DynamicMatrixRef() = delete;
DynamicMatrixRef(const DynamicMatrixRef&) = default;
~DynamicMatrixRef() {}
};

template<typename T>
class DynamicMatrixColMajorRef
{
DynamicMatrixColMajorRef() = delete;
DynamicMatrixColMajorRef(const DynamicMatrixColMajorRef&) = default;
~DynamicMatrixColMajorRef() {}
};
}
// Define the templates for any instantiations we use:
%template(DynamicDoubleVectorRef) simple_math::DynamicVectorRef<double>;
%template(DynamicDoubleMatrixRef) simple_math::DynamicMatrixRef<double>;
%template(DynamicDoubleMatrixColMajorRef) simple_math::DynamicMatrixColMajorRef<double>;

%{
// Within the wrapper C++ code, we need these headers:
#include "simple_math/Defines.h"
#include "simple_math/EigenDefines.h"
%}

// Wrap definitions
%include "simple_math/Defines.h"

// As discussed above, we need to define a function that can create an Eigen::Ref for an numpy
// array. We do not need this code for anything other than Python wrapping, so we will just
// define it inline rather than in a .h file.
// Importantly, apply the numpy typemaps to the pattern of arguments we will use in the
// inline'ed functions below.
// See: https://numpy.org/doc/stable/reference/swig.interface-file.html#introduction
%apply (double* INPLACE_ARRAY1, int DIM1) {(double* const data, const uint64_t size)}
%apply (double* INPLACE_ARRAY2, int DIM1, int DIM2) {(double* const data, const uint64_t rows, const uint64_t cols)}
%inline
%{
// A helper function to map a 1D numpy array to an Eigen vector reference
static simple_math::DynamicVectorRef<double> as_vector(
  double* const data, const uint64_t size)
{
    return simple_math::DynamicVectorMap<double>(data, size);
}
// A helper function to map a 2D numpy array to an Eigen matrix reference
static simple_math::DynamicMatrixRef<double> as_matrix(
  double* const data, const uint64_t rows, const uint64_t cols)
{
    return simple_math::DynamicMatrixMap<double>(data, rows, cols);
}
%}

/* Right now, I intend to just use Eigen::Ref, but these could be useful if we need more conversions.

// Eigen matrices into Numpy arrays.
%include "eigen.i"

%eigen_typemaps(simple_math::Vector3D)
%eigen_typemaps(simple_math::Matrix3D)
%eigen_typemaps(simple_math::VoigtVector)
%eigen_typemaps(simple_math::VoigtMatrix)
%eigen_typemaps(simple_math::GeneralVector)
%eigen_typemaps(simple_math::GeneralMatrix)
*/
