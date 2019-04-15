#pragma once

#include "simple_math/EigenDefines.h"
#include "simple_math/Utility.h"
#include "Eigen/Eigenvalues"
#include "utility/Range.h"
#include "utility/ContainerSearch.h"

namespace simple_math
{
    static const Matrix3D voigt_to_matrix(const VoigtVector & v)
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
            m(1, 1) = v(1);
            m(1, 2) = m(2, 1) = v(3);
            m(2, 2) = v(2);
            return m;
            break;
        default:
            throw _UTIL_MSG_EXCEPT("Unsupported size of a Voigt vector.");
        }
    }

    using EigenValueVectorPair = std::pair<std::complex<double>, Eigen::Vector3cd>;

    static EigenValueVectorPair get_max_eigen_pair(const Matrix3D & m)
    {
        Eigen::EigenSolver<Matrix3D> solver(m);
        const auto eigenvalues = solver.eigenvalues();
        auto max_index_pair = find_max_real_part(eigenvalues);
        auto eigenvectors = solver.eigenvectors();
        return { max_index_pair.first, eigenvectors.col(max_index_pair.second) };
    }
}