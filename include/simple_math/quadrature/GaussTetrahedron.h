#pragma once

#include "Quadrature.h"
#include "QuadratureDefines.h"
#include "utility/Exceptions.h"

namespace simple_math
{
template<QuadratureOrder N>
class GaussTetrahedron final
{
  public:
    /// @brief Constructs static memory for 3D Gauss quadrature specialized for tetrahedra.
    ///
    /// @template N - order of integration (1 results in 1 Gauss point, 2 results in 4 Gauss points,
    ///               3 results 15 Gauss points)
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const GaussTetrahedron<N>& get_singleton()
    {
        static GaussTetrahedron<N> singleton;
        return singleton;
    }

    inline const double* xis() const
    {
        return xis_;
    }
    inline const double* etas() const
    {
        return etas_;
    }
    inline const double* zetas() const
    {
        return zetas_;
    }
    inline const double* weights() const
    {
        return weights_;
    }
    QuadratureIndex n_points() const;

    /// @brief Returns a reference to the quadrature that is convenient to pass around.
    Quadrature get() const
    {
        return {
            QuadratureType::Gauss3DTet, N, false, n_points(), xis(), etas(), zetas(), weights()
        };
    }

  private:
    GaussTetrahedron();
    ~GaussTetrahedron();

    GaussTetrahedron(const GaussTetrahedron&) = delete;
    GaussTetrahedron(GaussTetrahedron&&)      = delete;

    double* xis_;
    double* etas_;
    double* zetas_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
GaussTetrahedron<N>::~GaussTetrahedron()
{
    delete[] xis_;
    delete[] etas_;
    delete[] zetas_;
    delete[] weights_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fall back for other values of N not implemented

template<QuadratureOrder N>
inline QuadratureIndex GaussTetrahedron<N>::n_points() const
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

template<QuadratureOrder N>
inline GaussTetrahedron<N>::GaussTetrahedron()
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 1-point rule (N=1)

template<>
inline QuadratureIndex GaussTetrahedron<1>::n_points() const
{
    return 1;
}

template<>
inline GaussTetrahedron<1>::GaussTetrahedron()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    xis_[0]     = 0.25;
    etas_[0]    = 0.25;
    zetas_[0]   = 0.25;
    weights_[0] = 1.0 / 6;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 4-point rule (N=2)

template<>
inline QuadratureIndex GaussTetrahedron<2>::n_points() const
{
    return 4;
}

template<>
inline GaussTetrahedron<2>::GaussTetrahedron()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    zetas_   = new double[n_points()];
    weights_ = new double[n_points()];

    auto val1   = (5.0 - sqrt(5.0)) / 20;
    auto val2   = (5.0 + 3 * sqrt(5.0)) / 20;
    xis_[0]     = val1;
    etas_[0]    = val1;
    zetas_[0]   = val1;
    weights_[0] = 1.0 / 24;

    xis_[1]     = val2;
    etas_[1]    = val1;
    zetas_[1]   = val1;
    weights_[1] = 1.0 / 24;

    xis_[2]     = val1;
    etas_[2]    = val2;
    zetas_[2]   = val1;
    weights_[2] = 1.0 / 24;

    xis_[3]     = val1;
    etas_[3]    = val1;
    zetas_[3]   = val2;
    weights_[3] = 1.0 / 24;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 15-point rule (N=3)

template<>
inline QuadratureIndex GaussTetrahedron<3>::n_points() const
{
    return 15;
}

template<>
inline GaussTetrahedron<3>::GaussTetrahedron()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    xis_[0]     = 0.25;
    etas_[0]    = 0.25;
    zetas_[0]   = 0.25;
    weights_[0] = 16.0 / 810;

    auto val1      = (7.0 - sqrt(15.0)) / 34;
    auto val2      = (13.0 + 3.0 * sqrt(15.0)) / 34;
    auto weightVal = (2665.0 + 14.0 * sqrt(15.0)) / 226800;

    xis_[1]     = val1;
    etas_[1]    = val1;
    zetas_[1]   = val1;
    weights_[1] = weightVal;

    xis_[2]     = val2;
    etas_[2]    = val1;
    zetas_[2]   = val1;
    weights_[2] = weightVal;

    xis_[3]     = val1;
    etas_[3]    = val2;
    zetas_[3]   = val1;
    weights_[3] = weightVal;

    xis_[4]     = val1;
    etas_[4]    = val1;
    zetas_[4]   = val2;
    weights_[4] = weightVal;

    val1      = (7.0 + sqrt(15.0)) / 34;
    val2      = (13.0 - 3.0 * sqrt(15.0)) / 34;
    weightVal = (2665.0 - 14.0 * sqrt(15.0)) / 226800;

    xis_[5]     = val1;
    etas_[5]    = val1;
    zetas_[5]   = val1;
    weights_[5] = weightVal;

    xis_[6]     = val2;
    etas_[6]    = val1;
    zetas_[6]   = val1;
    weights_[6] = weightVal;

    xis_[7]     = val1;
    etas_[7]    = val2;
    zetas_[7]   = val1;
    weights_[7] = weightVal;

    xis_[8]     = val1;
    etas_[8]    = val1;
    zetas_[8]   = val2;
    weights_[8] = weightVal;

    val1      = (10.0 - 2.0 * sqrt(15.0)) / 40;
    val2      = (10.0 + 2.0 * sqrt(15.0)) / 40;
    weightVal = 20.0 / 2268;

    xis_[9]     = val2;
    etas_[9]    = val1;
    zetas_[9]   = val1;
    weights_[9] = weightVal;

    xis_[10]     = val1;
    etas_[10]    = val2;
    zetas_[10]   = val1;
    weights_[10] = weightVal;

    xis_[11]     = val1;
    etas_[11]    = val1;
    zetas_[11]   = val2;
    weights_[11] = weightVal;

    xis_[12]     = val1;
    etas_[12]    = val2;
    zetas_[12]   = val2;
    weights_[12] = weightVal;

    xis_[13]     = val2;
    etas_[13]    = val1;
    zetas_[13]   = val2;
    weights_[13] = weightVal;

    xis_[14]     = val2;
    etas_[14]    = val2;
    zetas_[14]   = val1;
    weights_[14] = weightVal;
}

}