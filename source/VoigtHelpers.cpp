#include "simple_math/VoigtHelpers.h"

#include "Eigen/Eigenvalues"
#include "simple_math/EigenDefines.h"
#include "simple_math/Utility.h"
#include "utility/ContainerSearch.h"
#include "utility/Range.h"

namespace simple_math
{
VoigtVector matrix_to_voigt(const Matrix3D& m)
{
    // Assert symmetry
    assert(m(0, 1) == m(1, 0));
    assert(m(0, 2) == m(2, 0));
    assert(m(2, 1) == m(1, 2));

    VoigtVector v(6);
    v(0) = m(0, 0);
    v(1) = m(1, 1);
    v(2) = m(2, 2);
    v(3) = m(1, 2);
    v(4) = m(0, 2);
    v(5) = m(0, 1);
    return v;
}

Matrix3D voigt_to_matrix(const VoigtVector& v)
{
    Matrix3D m;
    m.setZero();
    switch (v.size())
    {
        case 3: // 2D
            throw _UTIL_MSG_EXCEPT("2D conversion of Voigt has not been implemented.");
        case 6: // 3D
            m(0, 0) = v(0);
            m(0, 1) = m(1, 0) = v(5);
            m(0, 2) = m(2, 0) = v(4);
            m(1, 1)           = v(1);
            m(1, 2) = m(2, 1) = v(3);
            m(2, 2)           = v(2);
            return m;
            break;
        default: throw _UTIL_MSG_EXCEPT("Unsupported size of a Voigt vector.");
    }
}

EigenValueVectorPair get_max_eigen_pair(const Matrix3D& m)
{
    Eigen::EigenSolver<Matrix3D> solver(m);
    const auto eigenvalues = solver.eigenvalues();
    auto max_index_pair    = find_max_real_part(eigenvalues);
    auto eigenvectors      = solver.eigenvectors();
    return { max_index_pair.first, eigenvectors.col(max_index_pair.second) };
}
}
