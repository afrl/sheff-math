#pragma once

#include "Quadrature.h"
#include "QuadratureDefines.h"
#include "utility/Exceptions.h"

namespace simple_math
{
template<QuadratureOrder N>
class GaussTriangle final
{
  public:
    /// @brief Constructs static memory for 2D Gauss quadrature specialized for triangles.
    ///
    /// @template N - order of integration (1 results in 1 Gauss point, 2 results in 3 Gauss points,
    ///               3 results 4 Gauss points, and 4 results in 7 Gauss points)
    ///
    /// Note: thread-safe for compilers that fully support C++11 with thread-safe function-local
    /// static initializers (or "magic statics").
    static const GaussTriangle<N>& get_singleton()
    {
        static GaussTriangle<N> singleton;
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
    inline const double* weights() const
    {
        return weights_;
    }
    QuadratureIndex n_points() const;

    /// @brief Returns a reference to the quadrature that is convenient to pass around.
    Quadrature get() const
    {
        return {
            QuadratureType::Gauss2DTriangle, N, false, n_points(), xis(), etas(), nullptr, weights()
        };
    }

  private:
    GaussTriangle();
    ~GaussTriangle();

    GaussTriangle(const GaussTriangle&) = delete;
    GaussTriangle(GaussTriangle&&)      = delete;

    double* xis_;
    double* etas_;
    double* weights_;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// Template defintions

template<QuadratureOrder N>
GaussTriangle<N>::~GaussTriangle()
{
    delete[] xis_;
    delete[] etas_;
    delete[] weights_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
// Fall back for other values of N not implemented

template<QuadratureOrder N>
inline QuadratureIndex GaussTriangle<N>::n_points() const
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

template<QuadratureOrder N>
inline GaussTriangle<N>::GaussTriangle()
{
    throw _UTIL_MSG_EXCEPT_C("Integration not implemented for N = " + std::to_string(N));
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 1-point rule (N=1)

template<>
inline QuadratureIndex GaussTriangle<1>::n_points() const
{
    return 1;
}

template<>
inline GaussTriangle<1>::GaussTriangle()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    xis_[0]     = 1.0 / 3;
    etas_[0]    = 1.0 / 3;
    weights_[0] = 0.5;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 3-point rule (N=2)

template<>
inline QuadratureIndex GaussTriangle<2>::n_points() const
{
    return 3;
}

template<>
inline GaussTriangle<2>::GaussTriangle()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    const auto val1      = 0.5;
    const auto val2      = 0.0;
    const auto weightVal = 1.0 / 6;

    xis_[0]     = val1;
    etas_[0]    = val1;
    weights_[0] = weightVal;

    xis_[1]     = val1;
    etas_[1]    = val2;
    weights_[1] = weightVal;

    xis_[2]     = val2;
    etas_[2]    = val1;
    weights_[2] = weightVal;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 3-point rule (N=3)

template<>
inline QuadratureIndex GaussTriangle<3>::n_points() const
{
    return 4;
}

template<>
inline GaussTriangle<3>::GaussTriangle()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    const auto val1      = 0.2;
    const auto val2      = 0.6;
    const auto weightVal = 25.0 / 96;

    xis_[0]     = val1;
    etas_[0]    = val1;
    weights_[0] = weightVal;

    xis_[1]     = val1;
    etas_[1]    = val2;
    weights_[1] = weightVal;

    xis_[2]     = val2;
    etas_[2]    = val1;
    weights_[2] = weightVal;

    xis_[3]     = 1.0 / 3;
    etas_[3]    = 1.0 / 3;
    weights_[3] = -27.0 / 96;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
// 7-point rule (N=4)

template<>
inline QuadratureIndex GaussTriangle<4>::n_points() const
{
    return 7;
}

template<>
inline GaussTriangle<4>::GaussTriangle()
{
    xis_     = new double[n_points()];
    etas_    = new double[n_points()];
    weights_ = new double[n_points()];

    xis_[0]     = 1.0 / 3;
    etas_[0]    = 1.0 / 3;
    weights_[0] = 0.225 * 0.5;

    auto val1      = 0.101286507323;
    auto val2      = 0.797426985353;
    auto weightVal = 0.125939180544 * 0.5;

    xis_[1]     = val1;
    etas_[1]    = val1;
    weights_[1] = weightVal;

    xis_[2]     = val1;
    etas_[2]    = val2;
    weights_[2] = weightVal;

    xis_[3]     = val2;
    etas_[3]    = val1;
    weights_[3] = weightVal;

    val1      = 0.470142064105;
    val2      = 0.059715871789;
    weightVal = 0.132394152788 * 0.5;

    xis_[4]     = val1;
    etas_[4]    = val1;
    weights_[4] = weightVal;

    xis_[5]     = val1;
    etas_[5]    = val2;
    weights_[5] = weightVal;

    xis_[6]     = val2;
    etas_[6]    = val1;
    weights_[6] = weightVal;
}
}
